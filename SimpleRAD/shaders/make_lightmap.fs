#version 450 core

out vec4 FragColor;

in vec3 fsPos;
in vec3 fsNormal;
in vec2 fsUV;
in vec3 fsColor;
in float fsA;

layout(RGBA8) uniform image2D tex2D;

uniform sampler2D posTex;
uniform sampler2D nrmTex;
uniform sampler2D arfTex;
uniform sampler2D ligTex;

uniform int pass;

#define PI 3.14159265359f
#define inf 999.0f
#define shadows true

struct Ray { vec3 o; vec3 dir; };

float ray_cone(Ray r, float cosa, float height, vec3 tipPos) {
	vec3 axis = vec3(0, -1, 0);
	vec3 co = r.o - tipPos;

    float a = dot(r.dir,axis)*dot(r.dir,axis) - cosa*cosa;
    float b = 2. * (dot(r.dir,axis)*dot(co,axis) - dot(r.dir,co)*cosa*cosa);
    float c = dot(co,axis)*dot(co,axis) - dot(co,co)*cosa*cosa;

    float det = b*b - 4.*a*c;
    if (det < 0.) return inf;

    det = sqrt(det);
    float t1 = (-b - det) / (2. * a);
    float t2 = (-b + det) / (2. * a);

    // This is a bit messy; there ought to be a more elegant solution.
    float t = t1;
    if (t < 0. || t2 > 0. && t2 < t) t = t2;
    if (t < 0.) return inf;

    vec3 cp = r.o + t*r.dir - tipPos;
    float h = dot(cp, axis);
    if (h < 0. || h > height) return inf;

	vec3 hitPos = r.o + r.dir * t;
	if(distance(hitPos, r.o)<0.05) return inf; // cone requires special treatment -_-
    return distance(hitPos, r.o);
}

float ray_cube(Ray r, vec3 boxPos, float scale, float yAngle) {
	mat3 to_AABB = inverse(mat3(
		cos(yAngle), 0, -sin(yAngle),
		0, 1, 0,
		sin(yAngle), 0, cos(yAngle)
	));
	
	vec3 rayOrigin = to_AABB*(r.o-boxPos);
	vec3 rayDir = to_AABB*r.dir;

	vec3 boxMin = vec3(-1.0)*scale;
	vec3 boxMax = vec3(1.0)*scale;

	vec3 tMin = (boxMin - rayOrigin) / rayDir;
    vec3 tMax = (boxMax - rayOrigin) / rayDir;
    vec3 t1 = min(tMin, tMax);
    vec3 t2 = max(tMin, tMax);
    float tNear = max(max(t1.x, t1.y), t1.z);
    float tFar = min(min(t2.x, t2.y), t2.z);

    if(tNear >= tFar) return inf;

	vec3 hitpos = rayOrigin + rayDir * tNear;

	if(tNear<-0.001) return inf; // not using 0 because the ground is slightly inside the big cube, causing unfortunate bright seams around it
	return distance(hitpos, rayOrigin);
}

float ray_sphere(Ray r, vec3 spos, float sr) {
    float a = dot(r.dir, r.dir);
    vec3 so_ro = r.o - spos;
    float b = 2.0 * dot(r.dir, so_ro);
    float c = dot(so_ro, so_ro) - (sr * sr);
    
    float disr = b*b - 4.0*a*c; //discriminant
    
    if (disr < 0.0) return inf;

	float t = (-b - sqrt(disr))/(2.0*a);
	if(t<=0) return inf;

    vec3 hitpos = r.o + r.dir*t;
	float dist = distance(r.o, hitpos);

    return dist;
}

bool occluded(vec3 self, vec3 other) {
	float dist = inf;
	vec3 dir = normalize(other-self);
	Ray r = Ray(self, dir);

	// Sphere
	vec3 s_c = vec3(0.57837, 0.45849, 0.45161);
	float s_r = 0.2974565;
	dist = min(dist, ray_sphere(r, s_c, s_r));

	// Big Cube
	vec3 center = vec3(-0.1454, -0.5300, -0.2920);
	float scale = 0.4704405;
	float yAngle = 0.610865f;
	dist = min(dist, ray_cube(r, center, scale, yAngle));

	center = vec3(-0.26327, 0.09585, -0.33665);
	scale = 0.1701705;
	yAngle = -0.279253;
	dist = min(dist, ray_cube(r, center, scale, yAngle));

	// cone
	vec3 cone_tip = vec3(0.67992, -0.54876, -0.52052);
	float height = 0.45124;
	float cosa = 0.89879404629;
	dist = min(dist, ray_cone(r, cosa, height, cone_tip));

	return dist < inf && dist < distance(self, other);
}

void main()
{
	if(pass<=0) {
		if(fsPos.y>0.999) {
			imageStore(tex2D, ivec2(fsUV*imageSize(tex2D)), vec4(1.0));
		}
		else {
			imageStore(tex2D, ivec2(fsUV*imageSize(tex2D)), vec4(0.0, 0.0, 0.0, 1.0));
		}
	}
	else if(texture(ligTex, fsUV).a>=1.0f){
		float dim = imageSize(tex2D).x;
		vec2 self = fsUV;

		vec3 col = vec3(0.0);
		if(fsPos.y>0.999) col = vec3(1.0);

		for(int x=0; x<dim; x++) {
			for(int y=0; y<dim; y++) {
				vec2 other = vec2(x, y)/dim;

				vec3 pos_s = 2.0f*(texture(posTex, self).rgb - 0.5);
				vec3 pos_o = 2.0f*(texture(posTex, other).rgb - 0.5);

				vec3 self_to_other = pos_o - pos_s;
				float r = length(self_to_other);

				if(r<0.05) continue;

				self_to_other = normalize(self_to_other);

				vec3 n_s = 2.0*(texture(nrmTex, self).rgb - 0.5f);
				vec3 n_o = 2.0*(texture(nrmTex, other).rgb - 0.5f);

				float cos_s = dot(n_s, self_to_other);
				float cos_o = dot(n_o, -self_to_other);

				if(cos_s <= 0.0 || cos_o <= 0.0) continue;

				float view_factor = cos_s * cos_o * (1.0f / (PI*r*r));

				float ref = 0.8; // reflectivity

				float fpa = (dim*dim)*texture(arfTex, other).x; // -> fragments per unit area on other
				vec4 source = texture(ligTex, other);
				source.rgb = source.rgb / fpa;

				if(source.a<1.0) continue;

				if(shadows && occluded(pos_s, pos_o)) continue;

				col += source.rgb * fsColor * ref * view_factor;
			}
		}

		//col = vec3(0.0);
		//vec3 pos_s = 2.0f*(texture(posTex, self).rgb - 0.5);
		//if(occluded(vec3(0,0,-2.5), pos_s)) col = vec3(1.0f);

		imageStore(tex2D, ivec2(fsUV*imageSize(tex2D)), vec4(col, 1.0));
	}
}