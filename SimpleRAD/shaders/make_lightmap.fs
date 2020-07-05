#version 450 core

out vec4 FragColor;

in vec3 fsPos;
in vec3 fsNormal;
in vec2 fsUV;

layout(RGBA8) uniform image2D tex2D;

uniform sampler2D posTex;
uniform sampler2D nrmTex;
uniform sampler2D ligTex;

uniform int init;

#define PI 3.14159265359f

void main()
{
	if(init>0) {
		if(fsPos.y>0.99) {
			imageStore(tex2D, ivec2(fsUV*imageSize(tex2D)), vec4(1.0));
		}
	}
	else {
		float dim = imageSize(tex2D).x;
		vec2 self = fsUV;

		vec3 col = texture(ligTex, self).rgb;

		for(int x=0; x<dim; x++) {
			for(int y=0; y<dim; y++) {
				vec2 other = vec2(x, y)/dim;
				if(other!=self) {
					vec3 self_to_other = texture(posTex, other).rgb - texture(posTex, self).rgb;
					float r = length(self_to_other);

					self_to_other = normalize(self_to_other);

					vec3 n_s = 2.0*(texture(nrmTex, self).rgb - 0.5f);
					vec3 n_o = 2.0*(texture(nrmTex, other).rgb - 0.5f);

					float cos_s = dot(n_s, normalize(self_to_other));
					float cos_o = dot(n_o, -normalize(self_to_other));

					float view_factor = max(0.0, cos_s * cos_o * (1.0f / (PI*r*r)));

					float ref = 0.01f; // reflectivity

					vec3 source = texture(ligTex, other).rgb;

					col += source * ref * view_factor;
					//col = vec3(view_factor);
				}
			}
		}
		imageStore(tex2D, ivec2(fsUV*imageSize(tex2D)), vec4(col, 1.0));
	}
}