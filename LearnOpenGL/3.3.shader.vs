#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec4 ourColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

float rand(vec2 n) { 
    return fract(sin(dot(n, vec2(12.9898, 4.1414))) * 43758.5453);
}

float noise(float x, float y){
    vec2 p = vec2(x,y);
    vec2 ip = floor(p);
    vec2 u = fract(p);
    u = u*u*(3.0-2.0*u);
    
    float res = mix(
        mix(rand(ip),rand(ip+vec2(1.0,0.0)),u.x),
        mix(rand(ip+vec2(0.0,1.0)),rand(ip+vec2(1.0,1.0)),u.x),u.y);
    return res*res;
}

float interp(float a, float b, float x) {
    float ft = x * 3.1415927;
    float f = (1-cos(ft)) * .5;
    return a * (1-f) + b*f;
}

float smoothnoise(float x, float y) {
    float corners = (noise(x-1,y-1)+noise(x+1,y-1)+noise(x-1,y+1)+noise(x+1,y+1))/16;
    float sides   = (noise(x-1, y)+noise(x+1, y)+noise(x, y-1)+noise(x, y+1) )/8;
    float center  = noise(x, y) / 4;
    return corners + sides + center;
}

float interpNoise(float x, float y) {
    int intX = int(floor(x));
    float fracX = x - intX;

    int intY = int(floor(y));
    float fracY = y - intY;

    float v1 = smoothnoise(intX, intY);
    float v2 = smoothnoise(intX+1,intY);
    float v3 = smoothnoise(intX,intY+1);
    float v4 = smoothnoise(intX+1,intY+1);

    float r1 = interp(v1,v2,fracX);
    float r2 = interp(v3,v4,fracX);

    return interp(r1, r2, fracY);
}

float perlinNoise(float x, float y) {
    float amplitude = 10f;
    int octaves = 3;
    float roughness = 0.5f;
    float total  = 0;

    float d = float(pow(2, octaves-1));
    for (int i = 0; i<octaves;i++) {
        float freq = float(pow(2,i)/d);
        float amp = float(pow(roughness,i) * amplitude);
        total += interpNoise(x*freq,y*freq) * amplitude;
    }
    return total;
}

void main()
{
    float amplitude = 3f;
    vec3 temp;
    temp.x = aPos.x;
    temp.y = aPos.y;
    temp.z = aPos.z;

    float noiseVal = perlinNoise(temp.x,temp.y);
    gl_Position = projection * view * model * vec4(aPos.x, aPos.y, noiseVal, 1.0);
    if(noiseVal < 9f)
    {
        ourColor = vec4(0.2f , 0.55f, 0.17f, 1.0f);
    }
    else if (noiseVal >= 9f && noiseVal < 10f)
    {
        ourColor = vec4(0.2f , 0.5f, 0.15f, 1.0f);
    }
    else if (noiseVal >= 10f && noiseVal < 11f)
    {
        ourColor = vec4(0.2f , 0.45f, 0.15f, 1.0f);
    }
    else if(noiseVal >= 11f && noiseVal < 12f)
    {
        ourColor = vec4(0.04f , 0.4f, 0.137f, 1.0f);
    }    
    else if(noiseVal >= 12f && noiseVal < 12.5f)
    {
        ourColor = vec4(0.2f , 0.4f, 0.137f, 1.0f);
    }
    else if(noiseVal >= 12.5f && noiseVal < 13f)
    {
        ourColor = vec4(0.6f, 0.52f, 0.25f, 1.0f);
    }
    else if(noiseVal >= 13f)
    {
        ourColor = vec4(0.8f, 0.52f, 0.25f, 1.0f);
    }
}