# To make the propeller protections with Illustrator and Tinkercad:

## Step 1: Illustrator

### 1.1 Ellipses and Alignment

To begin, create a new worktop with dimensions of 100mm wide by 100mm high,  
check that the units are in Millimeters, in "Advanced" choose "Raster Effects" "High 300 ppi".  
(and in the version 20.1.0 or below of Illustrator uncheck the option "Align the new objects on the grid in pixels" )

![Alt Text](Gifs2/05.gif)

Select the "Ellipse" tool in the left side panel while holding the mouse button on the "Rectangle" icon.  
Then in the window that appears click on Ellipse (or by pressing the L key on the keyboard)  
(If the "Tools Panel" is not displayed, go to the top menu and click on Window > Tools > Default)
  
Make a simple click on the blank page to bring up a dialog box.  
Enter the dimensions 70mm width and 70mm height.  
(The icon to the right of the dimensions boxes allow to links the proportions)

<img src="Gifs2/Ellipse_Box.png">

Change the appearance of the circle with the options at the top left of the document.  
Next to "Ellipse" click on the first box and choose the black square, in the box immediately to the right select the square crossed in red called "None" this allows to obtain a transparent outline.

![Alt Text](Gifs2/06.gif)

Using the same method, create another 65 mm by 65 mm circle.  
This time change the color of the circle by selecting the square white color, then in the box immediately to the right select the "None" square.

Now click on the "Select" tool in the left side panel to be able to select and manipulate the position of the circles freely.  
(Or by pressing the V key on the keyboard)

![Alt Text](Gifs2/07.gif)

Display the "Align" panel by selecting from the menu bar at the top of the screen,  
Clic on "Window" then on "Alignment" from the drop-down menu.  
(You can freely manipulate the position of the Alignment panel above your page)
<img src="Gifs2/Alignement_Simple_Box.png">

We will now highlight both circles and then in the "Align" panel.  
Press the "Horizontal Align Center" button and finally "Vertical Align Center".

![Alt Text](Gifs2/08.gif)

The white circle is now in the middle of the black circle.

### 1.2 Square, Advanced Alignment and Pathfinder

Now add a square of width 7mm and height 50mm

Align the square with the "Align" tools at the edge to the right of the black circle.  
To perform this action without changing the position of the circles, the additional alignment options must be displayed.  
To do this, click on the button at the top right of the alignment tool window <img src="Gifs2/Options.svg" style="float : right;margin-right: 7px;" width="20" height="20">
Choose "Show Options"  
An additional row of icons is displayed:

<img src="Gifs2/Alignement_Box.png">

Select by holding the left mouse button, only the black circle and the rectangle.  
Then choose at the bottom right of the Align Panel "Align To:" and click on "Align to Key Object".  
Highlight the black circle and press the buttons :  
"Horizontal Align Left" then "Vertical Align Center" in the "Align Objects" cathegory.

![Alt Text](Gifs2/09.gif)

We will now use the Pathfinder to subtract the square from the white circle.  
The Pathfinder is similar to the "Group" function in Tinkercad as it allows to perform boolean operator actions.

The Pathfinder is by default right next to "Align" in the Panel.  
(If not, display the Pathfinder panel by selecting from the menu bar at the top of the screen,
"Window" then "Pathfinder" drop-down menu)

<img src="Gifs2/Pathfinder_box.png">

Highlight the circle and rectangle by executing a (⇧ SHIFT + click) and then in the Pathfinder window use the function "Subtraction"

![Alt Text](Gifs2/10.gif)

The white circle now has an indentation on the left

### 1.3 Saving

Save the document by selecting in the top menu bar "File" then "Save As ..."

In the dialog box, you can freely name and choose where to save your file.  
It will nevertheless be necessary to select the "SVG" type of file from the drop-down menu next to "Format".  
Then press OK

Another dialog box appears, leaving the SVG Profile in SVG 1.1, you do not need to change the other options.  
Click OK

![Alt Text](Gifs2/11.gif)

## Step 2: Tinkercad

Import your file into Tinkercad using the button at the top right

In import options, click on "Artboard" then enter 100mm in Length and 100mm in Width

![Alt Text](Gifs2/12.gif)

Change the accuracy of the grid to 0.5mm

Add a cube 7.00mm high, 7.00mm depth by 10.00mm wide and a "hole" cube of 3.50mm high by 6.50mm wide

Change the height of the imported part to 2.00mm

![Alt Text](Gifs2/13.gif)

Align the 2 pieces on the flat part of the circle with the "Align" tool as seen in the previous tutorial.

![Alt Text](Gifs2/14.gif)

Take the hole cube and lengthen it by 3.00mm.
Raise it by 1.50mm and give it an incline of 8º to the outside.

(To be able to modify the inclination degree by degree, after clicking on the double arrow, you must keep the left click of the mouse pressed above the blue wheel above.)

![Alt Text](Gifs2/15.gif)

Select all the parts and group them together

Export the part by selecting "Export" then STL to be able to 3D print it !

![Alt Text](Gifs2/16.gif)
