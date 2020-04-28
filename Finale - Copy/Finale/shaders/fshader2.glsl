#version 400 core
//modifief from OpenGlLearn
out vec4 FragColor;

in vec2 TexCoords;
uniform int scene;

uniform sampler2D screenTexture;
uniform int pass;


//kernel 1
uniform float offset0;//0.003
uniform float m;//8

//kernel 2
uniform float offset1;//0.003
uniform float k;//9
uniform float n;//1

//edge detection
uniform float offset2;//0.003
uniform float e;//-8
uniform float d;//1

//blurry
uniform float offset3;//0.003
uniform float bl;//16
uniform float o;//1
uniform float t;//2





void main()
{
if(pass==2){

  if(scene==1){//normal

    vec3 col = texture(screenTexture, TexCoords).rgb;
    FragColor = vec4(col, 1.0);

    }
    else if(scene==2)//kernell test
    {
       vec2 offsets[9] = vec2[](
                vec2(-offset0,  offset0), // top-left
                vec2( 0.0f,    offset0), // top-center
                vec2( offset0,  offset0), // top-right
                vec2(-offset0,  0.0f),   // center-left
                vec2( 0.0f,    0.0f),   // center-center
                vec2( offset0,  0.0f),   // center-right
                vec2(-offset0, -offset0), // bottom-left
                vec2( 0.0f,   -offset0), // bottom-center
                vec2( offset0, -offset0)  // bottom-right    
                );

            float kernel[9] = float[](//kernell values
                          2, -4, 2,
                         -4,  m, -2,
                          2, -4,  2
                       );
    
            vec3 sampleTex[9];
            for(int i = 0; i < 9; i++)
            {
                sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
            }
            vec3 col = vec3(0.0);
            for(int i = 0; i < 9; i++)
                 col += sampleTex[i] * kernel[i];
    
             FragColor = vec4(col, 1.0);
    }
    else if(scene==3){//kernel effect
        
            vec2 offsets[9] = vec2[](
                vec2(-offset1,  offset1), // top-left
                vec2( 0.0f,    offset1), // top-center
                vec2( offset1,  offset1), // top-right
                vec2(-offset1,  0.0f),   // center-left
                vec2( 0.0f,    0.0f),   // center-center
                vec2( offset1,  0.0f),   // center-right
                vec2(-offset1, -offset1), // bottom-left
                vec2( 0.0f,   -offset1), // bottom-center
                vec2( offset1, -offset1)  // bottom-right    
                );

            float kernel[9] = float[](//kernell values
                         -n, -n, -n,
                         -n,  k, -n,
                         -n, -n, -n
                       );
    
            vec3 sampleTex[9];
            for(int i = 0; i < 9; i++)
            {
                sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
            }
            vec3 col = vec3(0.0);
            for(int i = 0; i < 9; i++)
                 col += sampleTex[i] * kernel[i];
    
             FragColor = vec4(col, 1.0);

         }

         else if(scene==4){//edge detection

         vec2 offsets[9] = vec2[](
                vec2(-offset2,  offset2), // top-left
                vec2( 0.0f,    offset2), // top-center
                vec2( offset2,  offset2), // top-right
                vec2(-offset2,  0.0f),   // center-left
                vec2( 0.0f,    0.0f),   // center-center
                vec2( offset2,  0.0f),   // center-right
                vec2(-offset2, -offset2), // bottom-left
                vec2( 0.0f,   -offset2), // bottom-center
                vec2( offset2, -offset2)  // bottom-right    
                );

            float kernel[9] = float[](//kernell values
                         d, d, d,
                         d, -e, d,
                         d, d, d
                       );
    
            vec3 sampleTex[9];
            for(int i = 0; i < 9; i++)
            {
                sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
            }
            vec3 col = vec3(0.0);
            for(int i = 0; i < 9; i++)
                 col += sampleTex[i] * kernel[i];
    
             FragColor = vec4(col, 1.0);

         }
     else if(scene==5){//blurry
          
          vec2 offsets[9] = vec2[](
                vec2(-offset3,  offset3), // top-left
                vec2( 0.0f,    offset3), // top-center
                vec2( offset3,  offset3), // top-right
                vec2(-offset3,  0.0f),   // center-left
                vec2( 0.0f,    0.0f),   // center-center
                vec2( offset3,  0.0f),   // center-right
                vec2(-offset3, -offset3), // bottom-left
                vec2( 0.0f,   -offset3), // bottom-center
                vec2( offset3, -offset3)  // bottom-right    
                );

           float kernel[9] = float[](//kernell values
                           o / bl, t / bl, o / bl,
                           t/ bl, 4.0 / bl, t / bl,
                          o / bl,  t/ bl, o / bl  
                        );
    
            vec3 sampleTex[9];
            for(int i = 0; i < 9; i++)
            {
                sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
            }
            vec3 col = vec3(0.0);
            for(int i = 0; i < 9; i++)
                 col += sampleTex[i] * kernel[i];
    
             FragColor = vec4(col, 1.0);

         }

   else//error
    {
      FragColor = vec4(1.0, 0.0, 1.0, 1.0); //error
     }

    }
} 