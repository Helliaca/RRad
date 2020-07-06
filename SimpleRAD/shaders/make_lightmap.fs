#version 450 core

out vec4 FragColor;

in vec3 fsPos;
in vec3 fsNormal;
in vec2 fsUV;
in vec3 fsColor;

layout(RGBA8) uniform image2D tex2D;

uniform sampler2D posTex;
uniform sampler2D nrmTex;
uniform sampler2D ligTex;

uniform int pass;

#define PI 3.14159265359f

void main()
{
	if(pass<=0) {
		if(fsPos.y>0.999) {
			imageStore(tex2D, ivec2(fsUV*imageSize(tex2D)), vec4(1.0));
		}
	}
	else {
		float dim = imageSize(tex2D).x;
		vec2 self = fsUV;

		vec3 col = vec3(0.0); //texture(ligTex, self).rgb;
		if(fsPos.y>0.999) col = vec3(1.0);

		for(int x=0; x<dim; x++) {
			for(int y=0; y<dim; y++) {
				vec2 other = vec2(x, y)/dim;

				vec3 pos_s = 2.0f*(texture(posTex, self).rgb - 0.5);
				vec3 pos_o = 2.0f*(texture(posTex, other).rgb - 0.5);

				vec3 self_to_other = pos_o - pos_s;
				float r = length(self_to_other);
				if(r<0.15) continue;

				self_to_other = normalize(self_to_other);

				vec3 n_s = 2.0*(texture(nrmTex, self).rgb - 0.5f);
				vec3 n_o = 2.0*(texture(nrmTex, other).rgb - 0.5f);

				float cos_s = dot(n_s, normalize(self_to_other));
				float cos_o = dot(-n_o, normalize(self_to_other));

				float view_factor = max(0.0, cos_s * cos_o * (1.0f / (PI*r*r)));

				float ref = 30.0f / (dim*dim); // reflectivity

				vec3 source = texture(ligTex, other).rgb;

				col += source * fsColor * ref * view_factor;
			}
		}
		imageStore(tex2D, ivec2(fsUV*imageSize(tex2D)), vec4(col, 1.0));
	}
}