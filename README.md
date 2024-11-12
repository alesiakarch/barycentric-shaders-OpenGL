[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/RM1pL2Qm)


ASE project idea
 
I am interested in painted shaders and creating an NPR look.

My initial idea for the project is developing a process that applies a procedural painted effect on the input meshes, so it can be used with any model. 
The effect needs to be artistically defined with a reference image, that will be "filter's" style. I am quite confident with producing or aquiring such images.

I have found that people have done a similar thing before in different ways. For example in 2021 Moon, Reddy and Tychonievich (https://history.siggraph.org/wp-content/uploads/2022/08/2021-Poster-40-Moon_Non-photorealistic-ray-tracing-with-paint-and-toon-shading.pdf )
used ray-tracing to find where the objects are in the assembled 3D scene and then projected a brush "filter" based on the original pixel colour. 

At the same time, there are several papers by Akleman et al (https://history.siggraph.org/wp-content/uploads/2022/10/2016-Poster-05-Du_harcoal-Rendering-and-Shading-With-Reflections.pdf
                                                             https://history.siggraph.org/wp-content/uploads/2022/06/2015-Labs-Akleman_Art-Directed-Rendering.pdf )
                                                             https://history.siggraph.org/wp-content/uploads/2022/09/2020-Poster-31-Subramanian_A-Painterly-Rendering-Approach.pdf )
that looked into developing NPR shaders themselves, alternating the input textures with the use of barycentric shaders. I think the charcoal shader is the closest to what I had in mind. 

Given this research, I have several options on which way I can implement the "effect": either as texture manipulation and shading or as general post-processing effect on the scene. 

Alternatively, if this idea is out of scope of this and furture units, I could try making a snow particle system with some user controlls as practice.  

