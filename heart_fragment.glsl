#version 330 core
in vec3 texCoord;
in vec2 test;
out vec4 fragColor;
uniform float uTime;

float dot2( in vec2 v ) { return dot(v,v); }

float sdHeart( in vec2 p )
{
    p.x = abs(p.x);

    if( p.y+p.x>1.0 )
        return sqrt(dot2(p-vec2(0.25,0.75))) - sqrt(2.0)/4.0;
    return sqrt(min(dot2(p-vec2(0.00,1.00)),
                    dot2(p-0.5*max(p.x+p.y,0.0)))) * sign(p.x-p.y);
}

void main()
{
    //mat2x2 Ry = mat2x2(1/cos(0*uTime), 0.0, 0.0, 1.0);
    //mat2x2 Rx = mat2x2(1.0, 0.0, 0.0, 1/cos(uTime));
    float ay = 1.5*uTime;
    float ax = 1.0*uTime;
    mat2x2 Ryx = mat2x2(cos(ay), sin(ay), -sin(ax)*sin(ay), cos(ax)-sin(ax)*cos(ay));
    vec3 pos = vec3( inverse(Ryx)*texCoord.xy, 0.0);
    /*
    if (length(pos.xy) > 0.5) {
        fragColor = vec4(1.0, 0.0, 0.0, 1.0);
    } else {
        fragColor = vec4(0.0, 1.0, 0.0, 1.0);
    }

    return;
    */
    float d = 0.05/abs(0.5-length(pos.xy));
    d = 1.0-sdHeart(pos.xy+vec2(0.0, 0.5));
    fragColor = vec4(d, 0.0, 0.0, 1.0);
    if (d >= 1.0) {
        float gb = 0.3;
        fragColor = vec4(1.0, gb, gb, 1.0);
    }
    //fragColor = vec4(0.5*(sin(uTime) + 1), r, g, 1.0);
}
