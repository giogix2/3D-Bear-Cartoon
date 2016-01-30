# 3D-Bear-Cartoon
##Interactive 3D scene, created with C++ and OpengGl, inspired by a cartoon.

It has been developed an application which draws a 3D object, composed by many parts, connected between them with hierarchical methods. The object is supposed to move with a simple animation. Another task of this application was the creation of a particle system feature, with constrains and collision detection. Finally a waves animation system has been developed, which simulate a pool with moving water in it.

![Alt text](https://github.com/giogix2/3D-Bear-Cartoon/blob/master/Images/Screen.jpg)

Here’s a brief summary of features of this application:

Used Blender to design the bear, and the pool
* Item The bear’s components are put together with a hierarchical system
* Item The bear moves automatically to the left and then to the right
* Item The harm of the bear can be moved with keyboard interaction (“l”)
* Item Simulation of the snow fog, made with a particle system
* Item Particles of the snow fog disappear slowly in the time and change position according to the gravity force
* Item Particles of the snow fog collide with the floor, which means that when those touch the floor, bounce in the opposite direction.
* Item The pool has moving water inside.

###Techniques
* Item **Hierarchical system:** The data structure used for the hierarchical system a tree. The root represent the main component, which in our case is the bear’s torso.Each node has 2 pointers, one to his child and one for his sibling. To draw all the part of the 3D object the traverse algorithm is used, to analyze each node just one time. The nodes, instead of the classical “struct”, have been represented by objects.