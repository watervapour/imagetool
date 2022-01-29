# Image program

This is planned to be an image file viewer and editor.
It is intended to be able to show detailed information of the raw binary data of a file.
This program should be able to display images, but also work in a 'cli mode'.
This program will initialy support:
 - .ppm
 - .png
Other formats such as tif, jpg, tga and other formats are stretch goals, dependant on imformation available.

## Motivation and features
This program is motivated by the unwieldy output of the rtoneweek project.
This is also intended to be a practice of skill, so only official format specifications will be used as resources for image formats.

	Features to be added (in roughly this order) are:
	 - output meta-data of .ppm to a file
	 - display a .ppm
	 - output meta-data of .png to a file
	 - display a .png
	 - writing of colour data to a file
	 - allow saving between formats
	=== Stretch goals ===
	 - add further image formats
	  - tif, jpg, tga, bmp
	 - auto-detection of file format
	 - display meta-data in GUI
	 - edit colour pallettes (as found in some 'modes' of PNG)
	  - higlight (with outlining or flashing) the pixels that will be edited
	 - show CIE chromacity diagram, when appropriate
	 - edit gamma and other details
	 - draw on 'canvas'
	 - draw into terminal with ascii / console control codes for colour
	 - custom backgrounds (for showing behind transparencies)
	 - support streamed data (such as interlaced/progressive mode for PNG)
	 - have multiple files at once (just as tabs like in Krita)
	 - other visual media related matters of interest:
	  - Sobel edge-detection filters
	  - Stereograms (may require openGL / 3D capabilities)

## Planned usage
As PNG is the core focus, some functionality / classes may be structured around how PNG operates.
When more formats are added, more abstraction may be implemented.
SDL will be used as the main/only external dependancy, and purely for creation of frame buffers, and handling platform specific window/display details.
Ideally, the core functionality should be contained in a library-like format, such that just the encoding/decoding functions can be used in other projects. This would be similar to the 'stb' image library.

	Functionality of the program should include:
	 - piping in (and out?) of a file
	 - argv
	  - "--input=ppm --of=png"

	Image_Format class:
	 - encode and decode functions 

	Image object:
	 - specifies format
	 - generic private members with getters:
	  - width / height
	  - colour bit depth
	  - author details, and other embedded meta-information

	Main program:
	 - Will need some form of GUI to handle:
	  - various text blocks
	  - mouse interactions
