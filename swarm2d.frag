#version 330 core
varying vec4 center;
uniform float radius;
uniform vec4 viewport;
uniform vec2 win;
void main(void)
{
    vec2 ndc_center = center.xy+win/2;
    vec2 diff = gl_FragCoord.xy - ndc_center;
    float d2 = dot(diff,diff);
    float r2 = radius*radius;
    float a = dot(center,center);
    a = center.z ==0 ? a/5000: a/1000;
    if (d2>r2) {
//        gl_FragDepth = center.z;
//        gl_FragColor = vec4(0.0,1.0,0,1.0);
        discard;
    } else {
        gl_FragColor = vec4((sin(a)+1)/2,(cos(a)+1)/2,0,1.0);
//        gl_FragColor = vec4(1.0,0.0,0,1.0);
    }


}

