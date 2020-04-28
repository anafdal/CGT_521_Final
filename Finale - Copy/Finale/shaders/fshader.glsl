#version 400 core

out vec4 FragColor;

//in vec3 ourColor;
//in vec2 TexCoord;

in vec3  TexCoord;
uniform vec4 Color;

uniform int choice;
uniform int pass;


//black and white
uniform float a;//0.2126
uniform float b;//0.7152


//inversion
uniform float sub;//1.0

// texture sampler
uniform samplerCube texture1;


void main()
{

  if(pass==1){
      
      //FragColor = texture(texture1, TexCoord);

       if(choice==1){//normal
       
          FragColor = texture(texture1, TexCoord)*Color;
        
	    }
        else if(choice==2){//inversion

            FragColor = vec4(vec3(sub - texture(texture1, TexCoord)), 1.0)*Color;
         }
       else if(choice==3){//black and white

              FragColor = texture(texture1, TexCoord);
              float average = a* FragColor.r + b * FragColor.g + 0.0722 * FragColor.b;
              FragColor = vec4(average, average, average, 1.0)*Color;
        } 

       }
    else
    {
      FragColor = vec4(1.0, 0.0, 1.0, 1.0); //error
     }
}