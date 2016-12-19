#version 120

uniform sampler2DRect u_positionsTex;

void main() {
    vec2 st = gl_MultiTexCoord0.xy;
    vec3 pos = texture2DRect(u_positionsTex, st).xyz;
    
    gl_PointSize = 1.0;
    gl_Position = gl_ModelViewProjectionMatrix * vec4(pos, 1.0);
    gl_FrontColor = gl_Color;
}