#version 120

#define PI 3.141592653589

uniform sampler2DRect u_positionsTex;
uniform float stepX;
uniform float stepY;

uniform float smoothing;
uniform float a;
uniform float b;
uniform float m;
uniform float n1;
uniform float n2;
uniform float n3;

vec3 sf3d(float x, float y) {
    float i = -PI + stepX * x;
    float j = -PI/2.0 + stepY * y;
    
    float raux1 = pow(abs(1.0/a * abs(cos(m * i/4.0))), n2) + pow(abs(1.0/b*abs(sin(m * i/4.0))), n3);
    float r1 = pow(abs(raux1), -1.0/n1);
    
    float raux2 = pow(abs(1.0/a * abs(cos(m * j/4.0))), n2) + pow(abs(1.0/b*abs(sin(m * j/4.0))), n3);
    float r2=pow(abs(raux2), -1.0/n1);
    
    float xx=r1*cos(i)*r2*cos(j)*100.0;
    float yy=r1*sin(i)*r2*cos(j)*100.0;
    float zz=r2*sin(j)*100.0;
    return vec3(xx,yy,zz);
}

void main() {
    vec2 st = gl_TexCoord[0].st;
    
    vec3 currentPos = texture2DRect(u_positionsTex, st).xyz;
    
    vec3 sf3dPos = sf3d(st.x, st.y);
    
    // smoothing
    vec3 diff = sf3dPos - currentPos;
    currentPos += diff * smoothing/100;
    
    gl_FragData[0] = vec4(currentPos, 1.0);
}
