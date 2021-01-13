#include <stdio.h>
#include <iostream>
#include "window.h"
#include "vec3.h"
#include "raytracer.h"
#include "sphere.h"
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define degtorad(angle) angle * MPI / 180

void save_image(int w, int h, std::vector<Color> framebuffer)
{
    std::cout << "saving image" << "\n";

    unsigned char data[w * h * 3];
    int frameIndex = 20000;
    int index = 0;
     for (int j = h - 1; j >= 0; --j)
     {
      for (int i = 0; i < w; ++i)
      {
       float r = framebuffer[frameIndex].r;
       float g = framebuffer[frameIndex].g;
       float b = framebuffer[frameIndex].b;
       int ir = int(255.0 * r);
       int ig = int(255.0 * g);
       int ib = int(255.0 * b);

       frameIndex--;

       data[index++] = ir;
       data[index++] = ig;
       data[index++] = ib;
      }
     }
     /*for (int i = 0; i < w; ++i){
        for (int j = h - 1; j >= 0; --j){
            float r = framebuffer[frameIndex].r;
            float g = framebuffer[frameIndex].g;
            float b = framebuffer[frameIndex].b;
            int ir = int(255.0 * r);
            int ig = int(255.0 * g);
            int ib = int(255.0 * b);

            std::cout << j << " " << i << " IR: " << ir << " IG: " << ig << " IB: " << ib << "\n";

            frameIndex++;

            data[index++] = ir;
            data[index++] = ig;
            data[index++] = ib;
        } 
     }*/

     /*int index = 0;
     for (int j = h - 1; j >= 0; --j)
     {
      for (int i = 0; i < w; ++i)
      {
       float r = (float)i / (float)w;
       float g = (float)j / (float)h;
       float b = 0.2f;
       int ir = int(255.0 * r);
       int ig = int(255.0 * g);
       int ib = int(255.0 * b);

       data[index++] = ir;
       data[index++] = ig;
       data[index++] = ib;
      }
     }*/


    stbi_write_jpg("jpg_test_.jpg", w, h, 3, data, 100);
}

int main(int argc, const char* argv[])
{ 
    if(argc <= 1){
        std::cout << "No arguments supplied!" << "\n";
        std::cout << "Arguments available:" << "\n";
        std::cout << "-w X = set image width to X" << "\n";
        std::cout << "-h X = set image height to X" << "\n";
        std::cout << "-r X = set rays per pixel to X" << "\n";
        std::cout << "-s X = set the amount of random spheres to appear in the scene to X" << "\n"; 
        std::cout << "Usage example: " << "\n";
        std::cout << "trayracer.exe -w 200 -h 100 -r 1 -s 4" << "\n";
    }
    Display::Window wnd;


    for (int i = 0; i < argc; i++)
    {
        std::cout << argv[i] << "\n";
    }
    
    wnd.SetTitle("BrayKracer");
    
    if (!wnd.Open())
        return 1;

    std::vector<Color> framebuffer;

    const unsigned w = std::stoi(argv[2]);
    const unsigned h = std::stoi(argv[4]);
    framebuffer.resize(w * h);
    
    int raysPerPixel = std::stoi(argv[6]);
    int maxBounces = 120;

    

    Raytracer rt = Raytracer(w, h, framebuffer, raysPerPixel, maxBounces);

    // Create some objects
    Material* mat = new Material();
    mat->type = "Lambertian";
    mat->color = { 0.5,0.5,0.5 };
    mat->roughness = 0.3;
    Sphere* ground = new Sphere(1000, { 0,-1000, -1 }, mat);
    rt.AddObject(ground);
    
    mat = new Material();
    mat->type = "Conductor";
    mat->color = {1,1,1};
    mat->roughness = 0.2f;
    Sphere* test = new Sphere(1, { 0,1,0 }, mat);
    rt.AddObject(test);

    mat = new Material();
    mat->type = "Lambertian";
    mat->color = { 0,0.4,0.6 };
    mat->roughness = 0.2;
    test = new Sphere(1, { -4,1,0 }, mat);
    rt.AddObject(test);

    mat = new Material();
    mat->type = "Dielectric";
    mat->color = { 1,0.8,0.7 };
    mat->roughness = 0.95;
    mat->refractionIndex = 1.65;
    test = new Sphere(1, { -4,1, 2 }, mat);
    rt.AddObject(test);

    mat = new Material();
    mat->type = "Lambertian";
    mat->color = { 1,0,0.2 };
    mat->roughness = 0.04;
    test = new Sphere(1, { 1,1, -3 }, mat);
    rt.AddObject(test);

    mat = new Material();
    mat->type = "Lambertian";
    mat->color = { 1,1,1 };
    mat->roughness = 0.0;
    test = new Sphere(1, { 4,1, 0 }, mat);
    rt.AddObject(test);

    bool exit = false;

    // camera
    bool resetFramebuffer = false;
    vec3 camPos = { 0.5, 4.0f, 7.0f };
    vec3 moveDir = { 0,0,0 };

    wnd.SetKeyPressFunction([&exit, &moveDir, &resetFramebuffer](int key, int scancode, int action, int mods)
    {
        switch (key)
        {
        case GLFW_KEY_ESCAPE:
            exit = true;
            break;
        case GLFW_KEY_W:
            moveDir.z -= 1.0f;
            resetFramebuffer |= true;
            break;
        case GLFW_KEY_S:
            moveDir.z += 1.0f;
            resetFramebuffer |= true;
            break;
        case GLFW_KEY_A:
            moveDir.x -= 1.0f;
            resetFramebuffer |= true;
            break;
        case GLFW_KEY_D:
            moveDir.x += 1.0f;
            resetFramebuffer |= true;
            break;
        case GLFW_KEY_SPACE:
            moveDir.y += 1.0f;
            resetFramebuffer |= true;
            break;
        case GLFW_KEY_LEFT_CONTROL:
            moveDir.y -= 1.0f;
            resetFramebuffer |= true;
            break;
        default:
            break;
        }
    });

    float pitch = 0;
    float yaw = 0;
    float oldx = 0;
    float oldy = 0;

    wnd.SetMouseMoveFunction([&pitch, &yaw, &oldx, &oldy, &resetFramebuffer](double x, double y)
    {
        x *= -0.1;
        y *= -0.1;
        yaw = x - oldx;
        pitch = y - oldy;
        resetFramebuffer |= true;
        oldx = x;
        oldy = y;
    });

    float rotx = 0;
    float roty = 0;

    // number of accumulated frames
    int frameIndex = 0;

    int lol = 0;

    std::vector<Color> framebufferCopy;
    framebufferCopy.resize(w * h);

    // rendering loop
    while (wnd.IsOpen() && !exit) 
    {
        if(lol > 0){
            /*for(int i = 0; i < 100; i++){
                std::cout << i << " R: " << framebufferCopy[i].r << "\t";
                std::cout << i << " G: " << framebufferCopy[i].g << "\t";
                std::cout << i << " B: " << framebufferCopy[i].b << "\n";
            }*/
            std::cout << framebuffer.size() << "\n";

        }
        else
        {
        resetFramebuffer = false;
        moveDir = {0,0,0};
        pitch = 0;
        yaw = 0;

        // poll input
        wnd.Update();

        rotx -= pitch;
        roty -= yaw;

        moveDir = normalize(moveDir);

        mat4 xMat = (rotationx(rotx));
        mat4 yMat = (rotationy(roty));
        mat4 cameraTransform = multiply(yMat, xMat);

        camPos = camPos + transform(moveDir * 0.2f, cameraTransform);
        
        cameraTransform.m30 = camPos.x;
        cameraTransform.m31 = camPos.y;
        cameraTransform.m32 = camPos.z;

        rt.SetViewMatrix(cameraTransform);
        
        if (resetFramebuffer)
        {
            rt.Clear();
            frameIndex = 0;
        }

        //RTX ON
        rt.Raytrace();
        frameIndex++;

        // Get the average distribution of all samples
        {
            size_t p = 0;
            for (Color const& pixel : framebuffer)
            {
                framebufferCopy[p] = pixel;
                framebufferCopy[p].r /= frameIndex;
                framebufferCopy[p].g /= frameIndex;
                framebufferCopy[p].b /= frameIndex;
                p++;
            }
        }
        std::cout << "FrameIndex: " << frameIndex << "\n";

        glClearColor(0, 0, 0, 1.0);
        glClear( GL_COLOR_BUFFER_BIT );

        wnd.Blit((float*)&framebufferCopy[0], w, h);
        wnd.SwapBuffers();
        }
        
        
    }

    if (wnd.IsOpen())
        wnd.Close();
    save_image(w, h, framebufferCopy);

    return 0;
} 

