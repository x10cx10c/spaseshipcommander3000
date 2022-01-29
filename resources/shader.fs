#version 100

precision mediump float;

// Input vertex attributes (from vertex shader)
varying vec2 fragTexCoord;
varying vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;
uniform vec2 iResolution;
uniform float iTime;

// NOTE: Add here your custom variables

// void main()
// {
//     // // Texel color fetching from texture sampler
//     // vec4 texelColor = texture2D(texture0, fragTexCoord)*colDiffuse*fragColor;

//     // texelColor.rgb *= 0.5;

//     // // Calculate final fragment color
//     // gl_FragColor = texelColor;


//     //     // Normalized pixel coordinates (from 0 to 1)
//     vec2 uv;
//     uv.x =  fragTexCoord.x/iResolution.x;
//     uv.y =  fragTexCoord.y/iResolution.y;

//     // // Time varying pixel color
//     vec3 col = 0.5 + 0.5*cos(iTime+uv.xyx+vec3(0,2,4));

//     // // Output to screen
//     gl_FragColor = vec4(col,1.0);
// }

#define pixelWidth 1.0/iResolution.x
#define offset_a pixelWidth * 0.5
#define offset_b pixelWidth * 0.8

struct Light {
    vec3 color;
    vec2 pos;
    float min;
    float max;
    float brightness;
};

//Two Lights, light0 is controlled by the mouse, light1 moves in a sine wave
//                   { Color },          {Position},    {Min},{Max},{Brightness}
Light light0 = Light(vec3(1.0, 1.0, 1.0), vec2(0.5, 0.5), 0.0, 2.5, 1.1);
Light light1 = Light(vec3(1.0, 1.0, 1.0), vec2(0.5, 0.6), 0.0, 2.5, 0.1);
Light light2 = Light(vec3(1.0, 1.0, 1.0), vec2(0.5, 0.7), 0.0, 2.5, 0.1);
Light light3 = Light(vec3(1.0, 1.0, 1.0), vec2(0.5, 0.8), 0.0, 2.5, 0.1);
Light light4 = Light(vec3(1.0, 1.0, 1.0), vec2(0.5, 0.9), 0.0, 2.5, 0.1);
Light light5 = Light(vec3(1.0, 1.0, 1.0), vec2(0.5, 0.4), 0.0, 2.5, 0.1);
Light light6 = Light(vec3(1.0, 1.0, 1.0), vec2(0.5, 0.3), 0.0, 2.5, 0.1);
Light light7 = Light(vec3(1.0, 1.0, 1.0), vec2(0.5, 0.2), 0.0, 2.5, 0.1);
Light light8 = Light(vec3(1.0, 1.0, 1.0), vec2(0.44, 0.5), 0.0, 2.5, 0.1);
Light light9 = Light(vec3(1.0, 1.0, 1.0), vec2(0.65, 0.6), 0.0, 2.5, 0.1);

float hyperstep(float min, float max, float x) {
    	            
    if (x < min) {
        return 1.0;
    }
    else if (x > max) {
        return 0.0;
    }
    else {
        //linear interpolation of x between min and max
        float value= (x - min) / (max - min);

        //hyperbolic function: 100/99 * (9x + 1)^2 - 1/99
        return (100./99.) / ((9. * value + 1.) * (9. * value + 1.)) - (1./99.);
    }
}

vec3 calculateLighting(vec2 pixel, Light light) {   
    return hyperstep(light.min, light.max,  distance(pixel, light.pos)) * light.brightness * light.color;
}

vec3 multisample(vec2 pixel) {
 
    vec2 points[4];
    
    points[0] = pixel + vec2(offset_a, offset_b);
    points[1] = pixel + vec2(-offset_a, -offset_b);
    points[2] = pixel + vec2(offset_b, -offset_a);
    points[3] = pixel + vec2(-offset_b, -offset_a);
    
    vec3 color = vec3(0.0);
    
    for (int i= 0; i < 4; i++) {
        color+= calculateLighting(points[i], light0);
        // color+= calculateLighting(points[i], light1);
        // color+= calculateLighting(points[i], light2);
        // color+= calculateLighting(points[i], light3);
        // color+= calculateLighting(points[i], light4);
        // color+= calculateLighting(points[i], light5);
        // color+= calculateLighting(points[i], light6);
        // color+= calculateLighting(points[i], light7);
        // color+= calculateLighting(points[i], light8);
        // color+= calculateLighting(points[i], light9);
    }
    
    return color / 4.0;
}


void main()
{
    
    vec2 pixel;// = fragTexCoord / iResolution.y;
    pixel.x =  fragTexCoord.x/iResolution.x;
    pixel.y =  fragTexCoord.y/iResolution.y;
    
    //background color
    vec3 color = fragColor.rgb; // blue
    color.rgb *= 0.2;

    //anti-aliasing
    color+= multisample(pixel);
    // if ( fragTexCoord.x > 0.5 ) color.rgb *= 0.2;
    
    if ( fragTexCoord.y < 0.35960 ) color.rgb *= 0.2;
    else if ( fragTexCoord.y < 0.3599 ) color = vec3(0.8,0.0,0.0);
    else if ( fragTexCoord.y < 0.4300 ) color = vec3(0.0,0.0,1.0);
    else if ( fragTexCoord.y < 0.4325 ) color = vec3(0.0,0.8,0.0);


    //x from 0.32031 to 0.33595
    //else if ( fragTexCoord.x == 0.0 ) color = vec3(0.5,0.5,0.5);
    
    gl_FragColor = vec4(color, 0.5);
}