# 3D-Bear-Cartoon
##Interactive 3D scene, created with C++ and OpengGl, inspired by a cartoon.

It has been developed an application which draws a 3D object, composed by many parts, connected between them with hierarchical methods. The object is supposed to move with a simple animation. Another task of this application was the creation of a particle system feature, with constrains and collision detection. Finally a waves animation system has been developed, which simulate a pool with moving water in it.

![Alt text](https://github.com/giogix2/3D-Bear-Cartoon/blob/master/Images/Screen.jpg)

Here’s a brief summary of features of this application:

Used Blender to design the bear, and the pool
* The bear’s components are put together with a hierarchical system
* The bear moves automatically to the left and then to the right
* The harm of the bear can be moved with keyboard interaction (“l”)
* Simulation of the snow fog, made with a particle system
* Particles of the snow fog disappear slowly in the time and change position according to the gravity force
* Particles of the snow fog collide with the floor, which means that when those touch the floor, bounce in the opposite direction.
* The pool has moving water inside.

###Techniques
* **Hierarchical system:** The data structure used for the hierarchical system a tree. The root represent the main component, which in our case is the bear’s torso.Each node has 2 pointers, one to his child and one for his sibling. To draw all the part of the 3D object the traverse algorithm is used, to analyze each node just one time. The nodes, instead of the classical “struct”, have been represented by objects.
* **Animation:** It has simply used a translation, which every time increases or decreases its values (in each frame). To arm is moved pressing the “l” key. The relative hand moves automatically thanks to the hierarchical system.
* **Particle system:** each particle is represented by a small quad. A prefixed number of particles is printed. Each particles has a random velocity and direction. During the time the particle disappears using the alpha channel of OpenGL. When a particle dies another one can is created. The collision system checks if the particle is going lower than the floor, and in that case it changes the direction of the particle.
* **Wave’s animation:** To simulate moving water it has been created a height map that is a grid composed by triangles. The height map is quad shaped and is drawn using the glBegin(GL_TRIANGLE_STRIP). Each vertex of the grid, when changed, automatically affects the position of the surrounding triangles. The movement of the water has been simulated using a 3D sinusoidal function, which changes values at each frame (amplitude and frequency can be set). The materials have been chosen to simulate the water has much as possible.