# CPU Ray Marching Renderer with minimal external dependencies

## Introduction

For computer graphics applications, there are two well-known renderer types.

One being Rasterizer, which is great for real-time renderering, and is often being implemented through hard-ware, a famous example is OpenGL pipeline. Video games or interactive applications in general mostly use this kind of renderer.

The second is Ray tracing, mostly used for generating great quality images, usually physics-based. With this approach, users care less about frame-rate, or computational time. But the quality of the results will speak for themselves. This approach is usually used in animation or movie industry.

However, there exists a third type of renderer, a "Ray marching" renderer. It adapts a different approach, using [SDF (Signed Distance Function)](https://iquilezles.org/www/articles/distfunctions/distfunctions.htm) to replace the collision information needed in ray tracing.

Ray marching has some serious disadvantages, including but not limited to...

* Slower than rasterizer
* Less "realistic" than ray tracing

On the otehr hand, it certainly has some interesting features, some of them are...

* Elegant, SDF can represent many geometries, including some fantastic fractals
* Easy-to-implement (and sometimes cheap, or even free to compute!) mesh operations, including masking, blending, and also edge detection

The technique of ray marching actually exists in many other applications in volume rendering, like volumetric light, 2D shadows, and many algorithms that requires computation on how the light transmitted in a dense medium like fog, cloud or space full of particles.

## Motivation

I like computer graphics and rendering a lot. Along my path I started with OpenGL, which is a rasterizer kind of renderer. Earlier this year I started a very simple ray tracer side project by following and improving Peter Shirley's ray tracing tutorial.

Until this moment I have had experience on the two most famous rendering techniques, and I was hoping some time I would be implementing another simple renderer using ray marching, and this is a great opportunity!

And since I have a working (simple, but working!) ray tracer at hand, I can do some analysis or observation about the image quality, characteristic, perofrmance...etc between the two renderers, and it sounds pretty cool!


## Target / Design Choice

### Determined direction

* Animation-oriented, abandon interactivity and focus on producing nice quality videos
* Have minimal dependencies, this way building it will be easy, and thus much more friendly to novice programmers that are trying to jump in the field of computer graphics
    * However, I am thinking about using stb_image for image save/load, and ffmpeg for generating videos
    * stb_image is a single header library, and do not require extra set ups, so it probably won't complicate stuffs
    * ffmpeg can be used as an external tool, and be used with shell scripts, thus not affecting the build chain, so it should be okay, too

### TBD

* Use physics-based shading or not?
* Is providing surface normal the responsibility of the surface, or should be evaluated by ray-casting?


## Implementation

My first instinct is to implement everything in c++, but having a python interface for setting up scenes sounds quite nice, so I might do that (if I have the time).

The first milestone will be just rendering some cubes or spheres on the image, then I can build extra features on top of it. Like lighting, materials, and mesh operations like masking and blending.


## Reference

* [SDF (Signed Distance Function)](https://iquilezles.org/www/articles/distfunctions/distfunctions.htm)
* [Peter Shirley's Ray tracing series](https://github.com/RayTracing/raytracing.github.io)