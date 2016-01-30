# 3D-Bear-Cartoon
###Interactive 3D scene, created with C++ and OpengGl, inspired by a cartoon.

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
* **Drawing:**
 * Using Blender to design the model of the tube, chair lift and the roof. Exported the models as .obj files. Created a parser in C++ (modified a parser found in internet), to import the .obj files. The materials attached to the object have been downloaded from internet.
 * Using the function glMatrixMode() to switch from GL_MODELVIEW matrix to the GL_PROJECTION matrix. In GL_MODELVIEW state it has been used the function glLookAt() to change the camera position (interactively with the keyboard). In GL_PROJECTION state it has been used the function glPerspective() to change the camera view (in particular the view angle can be changed using the keyboard).
 * Used the function glEnable(GL_CULL_FACE) to enable the cull face mode. Used the function glCullFace(GL_BACK) to hide the back side of the triangles (only the back side of the triangles composing the tube and the roof). Which is the back side of a polygon is decided depending on the direction in which the vertices are drawn. Vertices of the 3D object are drawn in clock-wise direction, so the back side correspond to the internal one, that are the non-visible one.
 * Used the function glRotate() and glTranslate() to move the object in the scene. To affect the modifies only to those object chosen, it has been used the functions glPushMatrix() and glPopMatrix().
 * Used the function glLightfv() to create a new light source. It has been created 4 different light source. One ambient light source, using the parameter GL_LIGHT_MODEL_AMBIENT, two diffuse light source, using the parameter GL_DIFFUSE and finally a specular light source, using the parameter GL_SPECULAR.
 * Used 3 different materials for the roof, 2 different materials for the chair lift and a single material for the tubes. The materials have been downloaded from internet in .blend format, imported in Blender and finally attached to the objects. The materials have been imported in the program scene using the same parser of the .obj objects, in particular those were contained in .mtl files.
* **Keyboard operations:**
 * Press “q” to increase the view angle of the glPerspective() function
 * Press “q” to decrease the view angle of the glPerspective() function
 * Press “e” to increase the z coordinate of the camera (eyeZ) of the glLookAt() function (i.e. moving the camera forward)
 * Press “d” to decrease the z coordinate of the camera (eyeZ) of the glLookAt() function (i.e. moving the camera backward)
 * Press “s” to decrease the x coordinate of the camera (eyeX) of the glLookAt() function (i.e. moving the camera to the left)
 * Press “f” to increase the x coordinate of the camera (eyeX) of the glLookAt() function (i.e. moving the camera to the right)
 * Press “r” to increase the y coordinate of the camera (eyeY) of the glLookAt() function (i.e. moving the camera up)
 * Press “g” to decrease the y coordinate of the camera (eyeY) of the glLookAt() function (i.e. moving the camera down)

###Implementation
**Hierarchical system**

To draw the 3D bear, first the model of all the components has been drawn using Blender. Each component of the bear (contained in a obj file), is loaded separately in the application. The data structure used for the hierarchical system is a tree (the structure is the one described in the book “Interactive Computer Graphics - A Top-Down Approach”, paragraph 8.4). Each node is an object, instead of a “struct” as described in the book. Each node has 2 pointers. One points to its child (which can be only one) and the other points to its sibling. All the node in the same level are pointed by the sibling pointer. To build this structure, in particular the object of the nodes, I’ve used the class objloader that I have already had in the previous assignments. I modified this class including the pointers (to child and siblings). In my case, the root of the tree, is represented by the bear’s torso. Its child is the bear’s head, which has the bear’s left arm as sibling, and so on. To draw all the 3D object I’ve used the traverse algorithm, also described in the book, which permits to draw each element just once. This kind of structure is useful for 2 reasons. First it works with all kind of composed objects, which means that the components can be changed and it will continue to work. Second once set the transformation of each component, moving the one in the higher level, it will make move even the one in the lower level accordingly.

**Hierarchical animation**

The hierarchical object is moved in 2 ways. First the root component is translated in the scene (to the left and to the right in the scene), proving that all the component are related between each other. In fact it’s not necessary to move each single component, but moving just the root (the bear’s torso), force all the part of the object to move together. Then there’s the possibility to control the movement ofthe bear’s arm, pressing the key “l”. The movement is done with a simple rotation, and the hand moves according to that.

**Particle system**

All the function and structures used to create the particle system are contained in the files ParticleSystem.h and ParticleSystem.cpp. To represent a single particle it has been used a “struct” which contains information about the position, direction, velocity, the time in which the particle has to be “alive”, a value which indicates if the particle is “alive”. These files contain also all the global variables used by the particle system (these have been moved here to don’t have too many in the main file). Initially the direction of the particles is set randomly (on the y and z axis) and all the particles are set as active. Each particle have the x and z values regarding the velocity set to 0, instead of the y value. This gives to the particles a similar behavior as the gravity. Each time the particles move, there will be a “force” (even if it’s not an acceleration bet velocity) which tries to move those down, simulating a real behavior of a moving object. During the time each particle slowly disappear changing the alpha value of the color, so after a certain time the particle disappear. After a certain time the particle dies, and a new one is generated. The particles a represented by simple quads, and the dimension of those can be changed with a global variable in the file. The color of the particles has been chosen white to simulate the snow. For the collision system, each time a particle goes down and reaches the height of the floor, the direction is changed in the opposite direction. So the particles cannot go through the floor.

**Waves animation**

To create the moving water it has been first created a height map. The height map is a grid, composed by many triangles, which permits to easily be deformed, just changing the position of a certain vertex. The code regarding the creation of the water is contained in the files Water.h and Water.cpp. This class contains information about the size of the grid, a 2 dimensional matrix with the heights of all the vertices and a 2 dimensional matrix containing the normal of each vertex. In fact the normal, instead of being applied to the surfaces of the triangles, are applied directly to the vertices, to have a better simulation of the waves (Gourad shading). After having created the height map is possible to set and get the height of each vertex with setter/getter methods offered by the class. This class provides also a function which automatically calculates the normal vectors of the vertices, finding the average value between the surrounding vectors. The normal vectors are then normalized. Finally the function draw draws all the vertices using theTRIANGLE_STRIP directive, which permits to easily draw the triangles of the grid just providing the appropriate vertices. Once the height map has been created the waves shape have been created with the 2 dimensional function sin⁡(_wavewidth_*_x_+_time_)* cos(_wavewidth_*_z_+_time_). This function is put in the drawScene function of OpenGL and the value of the variable time are increased at each frame. The variable waveWidth affects the size of the waves. The material of the waves have been chosen to give an aspects as close as possible to the real water, especially taking care of the reflectivity of it.

**Drawing 3D objects**

I’ve started creating the 3D model of a tube using Blender. I’ve just created a cylinder and modified the dimension of it. Then I’ve downloaded the material to attach from the following site: http://matrep.parastudios.de/ . In particular I’ve attached a material called “metal_shiny”, which represent a special kind of chrome metal. I’ve created just a vertical tube, which I used to draw the first tube. Then I’ve used the same model to draw the same tube and then I’ve rotated it and translated, to represent the oblique one.
Then I’ve created the model of the roof. To do that I’ve first created a cube, I’ve scalded it and blended it to create just the upper part of it. Then I’ve created 2 poles, deforming 2 cubes and finally I’ve attached a bench to it just putting 2 deformed cubes. The final result is shown in figure 2. For the upper part I’ve used a material called “walnut”, for the poles I’ve used the material “metal_shiny” and for the benches I’ve used the material “Wood”, all coming from the website http://matrep.parastudios.de/ .
![Alt text](https://github.com/giogix2/3D-Bear-Cartoon/blob/master/Images/Screen4.jpg)

Then I’ve created the model of the chair lift. To do that I’ve first created a tube, deformed it, and used as basic structure. Then I’ve attached 3 deformed cubes as benches. The figure 3 shows the Blender model of the chair lift.
![Alt text](https://github.com/giogix2/3D-Bear-Cartoon/blob/master/Images/Screen5.jpg)

For the structural tube it has been used the material called “alluminium_galvanized” and for the benches it has been used the material “Wood 3”, both also coming from the website http://matrep.parastudios.de/.

**objLoader (Class used to import .obj files)**

The models created in Blender were exported in .obj format (and .mtl format for the materials). OpenGL doesn’t have any functions to directly import this kind of files, and not many library are available to do that. So to import this files I’ve found a code online and I’ve modified to make it work. The code is well explained in a youtube’s video, present in the following link [link1](https://www.youtube.com/watch?v=XIVUxywOyjE) . In the bottom of the video there are also 3 links to the final code

**Implementation**

The implementation starts with a series of global variables of each 3D object imported from obj files. In the initRendering function the 3D object are loaded in these variables. All the code from the previous assignments hasn’t been changed. The class objloader takes care of loading the obj files and drawing those. It has been changed a little from the previous assignment since it doesn’t load the objects in vector anymore, but the objects of this class keep the information and draw those directly. The particles of the particle system are drawn with the function DrawParticles in the drawScene function. The particles are translated in the same way the bear is translated, to simulate the snow moved by the snowboard. The movement of the bear are done by the function moveBear contained in the ParticleSystem files. All the part of the bear are drawn by the function traverse contained in the objloader files.

**Extra**

I’ve implemented the possibility to change the head of the bear in real-time. When the program is running is possible to press the keys “o”, “p” and “i” to have 3 different head of the bear. I thought that this could be useful also to test how the hierarchical system work, and how it works regardless of the object loaded. In fact is sufficient to load a different 3D object to test the system keep working.

![Alt text](https://github.com/giogix2/3D-Bear-Cartoon/blob/master/Images/Screen2.jpg)
![Alt text](https://github.com/giogix2/3D-Bear-Cartoon/blob/master/Images/Screen3.jpg)

###Compiling and running

The objloader class uses the SDL.h file as dependency. The obj files have to be put in the same directory of the cpp and h files. In this assignment the version of glew used is changed, and it has been used glew 1.8.0 because of problems of compatibility. The library glm has also been used.

**Dependencies**

* Opengl version: 4.2
* glew.h
* glm
* ParticleSystem.h
* Shapes.h,
* vec3f.h
* Water.h
* objloader.h
* SDL

