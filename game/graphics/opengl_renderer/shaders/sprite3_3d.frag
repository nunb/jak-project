#version 430 core

out vec4 color;

in flat vec4 fragment_color;
in vec3 tex_coord;
in flat uvec2 tex_info;

uniform sampler2D tex_T0;
uniform float alpha_min;
uniform float alpha_max;

void main() {
    vec4 T0 = texture(tex_T0, tex_coord.xy);
    if (tex_info.y == 0) {
        T0.w = 1.0;
    }
    color = fragment_color * T0 * 2.0;



    if (color.a < alpha_min) {
        discard;
    }

    if (color.a > alpha_max) {
        discard;
    }
}

//out vec4 color;
//
//in flat vec4 fragment_color;
//in vec3 tex_coord;
//in flat uvec2 tex_info;
//
//uniform sampler2D tex_T0;
//
//
//void main() {
//    vec4 T0 = texture(tex_T0, tex_coord.xy);
//    if (tex_info.y == 0) {
//        T0.w = 1.0;
//    }
//    vec4 tex_color = fragment_color * T0 * 2.0;
//    if (tex_color.a < 0.016) {
//        discard;
//    }
//    color = tex_color;
//}
