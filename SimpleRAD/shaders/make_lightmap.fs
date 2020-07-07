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

struct Ray { vec3 o; vec3 dir; };

//bool BBoxIntersect(vec3 boxMin, vec3 boxMax, Ray r) {
//	vec3 tbot = r.invDir * (boxMin - r.o);
//	vec3 ttop = r.invDir * (boxMax - r.o);
//	vec3 tmin = min(ttop, tbot);
//	vec3 tmax = max(ttop, tbot);
//	vec2 t = max(tmin.xx, tmin.yz);
//	float t0 = max(t.x, t.y);
//	t = min(tmax.xx, tmax.yz);
//	float t1 = min(t.x, t.y);
//
//	//float dist = 
//
//	return t1 > max(t0, 0.0);// && t0>0.1f;
//	//return hit;
//}

bool ray_sphere(Ray r, vec3 spos, float sr, out float dist) {
    float a = dot(r.dir, r.dir);
    vec3 so_ro = r.o - spos;
    float b = 2.0 * dot(r.dir, so_ro);
    float c = dot(so_ro, so_ro) - (sr * sr);
    
    float disr = b*b - 4.0*a*c; //discriminant
    
    // Doesn't hit
    if (disr < 0.0) {
        return false;
    }
    // hits
    vec3 hitpos = r.o + r.dir*(-b - sqrt(disr))/(2.0*a);
	dist = distance(r.o, hitpos);
    return true;
}

bool things(vec3 self, vec3 other) {
	vec3 s_c = vec3(0.57837, 0.45849, 0.45161);
	float s_r = 0.2974565;

	vec3 dir = normalize(other-self);
	Ray r = Ray(self, dir);

	float dist;
	bool h = ray_sphere(r, s_c, s_r, dist);

	return h && dist>0.1;


	//vec3 center = vec3(0.0f, 0.5f, 0.0f);//vec3(-0.1454, -0.5300, -0.2920);
	//float scale = 0.1f;
	//float yAngle = 0.0;//-0.610865f;
	//
	//mat3 to_AABB = inverse(mat3(
	//	cos(yAngle), 0, -sin(yAngle),
	//	0, 1, 0,
	//	sin(yAngle), 0, cos(yAngle)
	//));
	//
	//Ray r;
	//r.o = self - center;
	//r.dir = normalize(other-self);
	//r.dir = to_AABB * r.dir;
	//r.invDir = -r.dir;
	//
	//return BBoxIntersect(vec3(-1.0)*scale, vec3(1.0)*scale, r);
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

				//if(things(pos_s, pos_o)
					//&& fract(pos_o.x) > 0.01
					//&& fract(pos_o.y) > 0.01
					//&& fract(pos_o.z) > 0.01
				//) col = vec3(other.x, other.y, 0.0);

				col += source.rgb * fsColor * ref * view_factor;
			}
		}

		//col = vec3(0.0);
		//vec3 pos_s = 2.0f*(texture(posTex, self).rgb - 0.5);
		//if(things(vec3(0,0,-2.5), pos_s)) col = vec3(1.0f);
		//col = vec3(fsA);

		imageStore(tex2D, ivec2(fsUV*imageSize(tex2D)), vec4(col, 1.0));
	}
}