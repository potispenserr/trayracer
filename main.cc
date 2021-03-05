// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include <stdio.h>
#include <iostream>
#include "vec3.h"
#include "raytracer.h"
#include "sphere.h"
#include <stdlib.h>
#include <chrono>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define degtorad(angle) angle * MPI / 180

void save_image(int w, int h, std::vector<Color>& framebuffer)
{
    std::cout << "saving image" << "\n";

    unsigned char data[w * h * 3];
    int index = 0;
    for (int i = 0; i < w * h; i++){
        data[index++] = 255.0f * framebuffer[i].r;
        data[index++] = 255.0f * framebuffer[i].g;
        data[index++] = 255.0f * framebuffer[i].b;

    }
    stbi_flip_vertically_on_write(1);

    stbi_write_jpg("jpg_test_.jpg", w, h, 3, data, 100);

    std::cout << "saving complete" << "\n";
}

    

int main(int argc, const char* argv[])
{ 
    srand(time(0));
    
    if(argc <= 1){
        std::cout << "No arguments supplied!" << "\n";
        std::cout << "Arguments available:" << "\n";
        std::cout << "-w X = set image width to X" << "\n";
        std::cout << "-h X = set image height to X" << "\n";
        std::cout << "-r X = set rays per pixel to X" << "\n";
        std::cout << "-s X = set the amount of random spheres to appear in the scene to X" << "\n"; 
        std::cout << "Usage example: " << "\n";
        std::cout << "./trayracer -w 200 -h 100 -r 1 -s 4" << "\n";
    }
    else{
        std::cout << "Starting" << "\n";
        std::vector<std::string> argList;
        for(int i = 0; i < argc; i++){
            argList.push_back(argv[i]);
        }
        
        std::vector<Color> framebuffer;

        const unsigned w = std::stoi(argList[2]);
        const unsigned h = std::stoi(argList[2]);
        framebuffer.resize(w * h);
        
        const int raysPerPixel = std::stoi(argList[6]);
        const int maxBounces = 5;

        

        Raytracer rt = Raytracer(w, h, framebuffer, raysPerPixel, maxBounces);

        // Create some objects
        Material mat;
        mat.type = "Lambertian";
        mat.color = { 0.5,0.5,0.5 };
        mat.roughness = 0.3;
        Sphere* ground = new Sphere(1000, { 0,-1000, -1 }, mat);
        rt.AddObject(ground);
        for(int i = 0; i < std::stoi(argList[8]); i++){
            double randX = rand() % 7;
            double randY = rand() % 5;
            double randZ = -(rand() % 5);

            int randType = rand() % 3;
            float randR = rand() % 2;
            float randG = rand() % 2;
            float randB = rand() % 2;
            
            switch (randType)
            {
            case 0:
                mat.type = "Conductor";
                break;
            case 1:
                mat.type = "Lambertian";
                break;
            case 3:
                mat.type = "Dielectric";
                break;

            default:
                break;
            }
            mat.color = {randR,randG,randB};
            mat.roughness = 0.2f;
            Sphere* test = new Sphere(1, { randX, randY, randZ}, mat);
            rt.AddObject(test);
        }

        // camera
        vec3 camPos = { 0.5, 4.0f, 7.0f };
        vec3 moveDir = { 0,0,0 };

        float rotx = 0;
        float roty = 0;

        std::vector<Color> framebufferCopy;
        framebufferCopy.resize(w * h);

        // rendering loop
        for(int i = 0; i < 1; i++) 
        {

            mat4 xMat = (rotationx(rotx));
            mat4 yMat = (rotationy(roty));
            mat4 cameraTransform = multiply(yMat, xMat);

            camPos = camPos + transform(moveDir * 0.2f, cameraTransform);
            
            cameraTransform.m30 = camPos.x;
            cameraTransform.m31 = camPos.y;
            cameraTransform.m32 = camPos.z;

            rt.SetViewMatrix(cameraTransform);
            
            //RTX ON
            auto t1 = std::chrono::high_resolution_clock::now();
            rt.Raytrace();
            auto t2 = std::chrono::high_resolution_clock::now();

            std::chrono::duration<double> secDuration = t2 - t1;

            // integral duration: requires duration_cast
            // converting integral duration to integral duration of shorter divisible time unit:
            // no duration_cast needed

            unsigned int rayAmount = w * h * raysPerPixel;
            std::cout << "rayAmount: " << rayAmount << "\n";
        
            std::cout << "RTX took " << secDuration.count() << " s" << " with " << (rayAmount / secDuration.count()) / 1000000 << " MRays/s" <<  std::endl;

            // Get the average distribution of all samples
            {
                size_t p = 0;
                for (Color const& pixel : framebuffer)
                {
                    framebufferCopy[p] = pixel;
                    //framebufferCopy[p].r /= 1;
                    //framebufferCopy[p].g /= 1;
                    //framebufferCopy[p].b /= 1;
                    p++;
                }
            }            
        }
        
        save_image(w, h, framebufferCopy);
        rt.ClearObjects();

        return 0;
    }
    
} 
