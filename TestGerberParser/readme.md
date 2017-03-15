This folder contains a test program for GerberParser.

It is a command line application, which produces a DXF output from one or more gerbers.

# Command Line arguments

Each argument has a tag beginning with - which may be followed by one or more further arguments.

|Tag|Description|
|---|:-----------|
|-folder folder|Specifies a folder which must contain files top_paste_mask.grb and drill_file.drl. An output is produced based on these files.|
|-params parameterfile|Specifies a parameter file which controls the output. If this is specified the -folder and -gerber arguments should not be.|
|-gerber gerberfile|Specifies a gerber file on which the output is based|
|-test outputfile|output is written in a debugging format to outputfile|
|-dxf outputfile|output is written in dxf format to outputfile|
|-Polyline|When outputting to dxf output polygons and linestrings using POLYLINE records|
|-LWPolyline|When outputting to dxf output polygons and linestrings as LWPOLYLINE records|
|-layer layersfile|Add the layers specified in layersfile to the output|

## -params parameterfile

The root node of the parameter file is config. 
Beneath config are:

|Element|Description|
|-------|:-----------|
|folders|has either a path element which specified another XML file which defines folders or one or more folder nodes|
|drawings|contains one or more drawing elements that appear in the output|
|inflate|default distance by which part outlines are inflated|
|deflate|default distance by which pads are deflated|
|minsize|minimum diameter for a hole in the drills file to be output as a mounting holes|
|cutline_inflate|amount by which the cutline (bounding box of drawing) is inflated|

#### Example parameter file

```
<?xml version="1.0" encoding="utf-8"?>
<config>
  <!-- all dimensions are in millimetres -->
  <!-- deflate is subtracted from the size of aperatures when drawing the paste mask-->
  <deflate>0.05</deflate>
  <!-- the bounding box of part outlines is inflated by the value of the inflate parameter -->
  <inflate>0.2</inflate>
  <!-- minsize specifies the minimum size of a drill holes to be included the drills pass -->
  <minsize>2.9</minsize>
  <!-- These global defaults can be overridden for individual paste , parts and drills elements -->
  <cutline_inflate>1</cutline_inflate>
  <!-- Set of folder paths to operate on -->
  <folders>
    <path>C:\DEV\ElectronicsProduction\TestGerberParser\Folders.xml</path>
  </folders>
  <!-- Specify each drawing to appear on the sheet -->
  <drawings>
    <drawing>
      <folder>1</folder>
      <X>35</X>
      <Y>50</Y>
      <paste/>
      <include>
        <path>C:\DEV\ElectronicsProduction\Stencil_Jig\holes.xml</path>
      </include>
      <include>
        <path>C:\DEV\ElectronicsProduction\Stencil_Jig\cutline.xml</path>
      </include>
    </drawing>
  </drawings>
</config>

```
The above file draws one drawing offset by 35,50 from the origin of the drawing.
The drawing includes the pads from the paste layer, together with standard graphics for holes and cutline.
This enables a solder paste stencil to be produced with standard holes and cut line to match the jig that holds the stencil.

### folders file

```
    <folder>
      <ID>4</ID>
      <path>C:\DEV\HomeAutomation\CAM\CAM_Temperature\CAM_Temperature</path>
      <parts>
        <part>
          <ID>OP1</ID>
          <W>0</W>
          <H>1</H>
        </part>
      </parts>
    </folder>

```
### folder node
The folder node has

|Element|Description|
|-------|:-----------|
|ID|Unique Integer used to identify folder|
|path|Path of folder contain gerber and drill files|
|parts|optional node beneath which are part nodes defining override outlines of parts|

The path specified in the folder node should contain top_paste_mask.grb and drill_file.drl

Each part node has

|Element|Description|
|-------|:-----------|
|ID|ID or part reference as it appears in comment text in the paste gerber|
|W|Distance by which the width of the outline of this part is inflated|
|H|Distance by which the height of the outline of this part is inflated|

## drawings node
The drawings node has drawing and / or include nodes beneath it.

### include node

The has a path node beneath it. The path node contains the path of a file which has a drawings node beneath config, which contains the drawings to be drawn.

### drawing node

A number of gerbers or other graphics can be placed in the output. This enables several masks to be output to a single DXF.
Each drawing is defined by a drawing node which has:

|Element|Description|
|-------|:-----------|
|folder|ID of folder to be drawn. If this is not specified some graphics should be specified for the node.|
|X|X offset that is added to every coordinate output|
|Y|Y offset that is added to every coordinate output|
|cutlineinflate|amount by which the cutline is inflated (overrides the default)|
|inflate|amount by which part outlines are inflated (overrides the default)|
|parts|draws part outlines for parts identified in the gerber|
|paste|draws pads from the Pads layer of the gerber file. This can have a deflate node beneath it, if specified this overrides the default|
|drills|draws holes whose diameter is above minsize to the Mounting_Holes layer|
|graphics|beneath this are a number of graphic nodes, which specify graphics to be output to the dxf|
|outline|draws the outline from the gerber to the Outline layer|
|cutline|draws the bounding box of the graphics in the gerber (inflated by cutlineinflate)|
|include|beneath this is a path node which specifies the path of a file defining some graphics|

### graphics

The graphics node has one or more elements beneath it that define graphics to be drawn.

These nodes may be:

|Node Name|Description|
|---------|:-----------|
|hole|Defines the position, diameter and layer of a hole to be drawn|
|geometry|Defines a polyline or polygon and layer to be drawn|

#### hole node

This has:

|Element|Description|
|-------|:-----------|
|X|X coordinate of centre of hole|
|Y|Y coordinate of centre of hole|
|diameter|diameter of hole|
|layer|layer on which hole is to be drawn|

#### geometry node

This has:

|Element|Description|
|-------|:-----------|
|layer|layer on which hole is to be drawn|
|geometry|geometry specified in Well Known Text (WKT) format|

WKT format is described here: https://en.wikipedia.org/wiki/Well-known_text

WKT is part of the Simple Feature Access specification http://www.opengeospatial.org/standards/sfa

Currently only POINT, LINESTRING and POLYGON types are supported.
POLYGONs must have a single ring.

### include node under drawing

The include node under drawing defines the path of a file containing graphics definitions and the layer these are to be drawn on.
It has

|Element|Description|
|-------|:-----------|
|path|path of file containing a top level graphics node the defines some graphics.|
|layer|name of the layer that graphics are to be drawn on|

## -Polyline and -LWPolyline
If neither of these is specified polygons and polylines are output as line records. If these files are viewed in LaserCut a polygon will appear as separate lines, which need to be joined together.

-Polyline outputs polylines, which do not have a count of coordinates. There aren't many readers that handle these.

-LWPolyline outputs LWPOLYLINE records which include a count of coordinates. If these files are viewed in LaserCut polygons appear correctly.

## -layers layerfile

layerfile is an XML file which specifies layers that appear in the output.
Each layer is specified by a layer element beneath a layers element, example

```
<?xml version="1.0" encoding="UTF-8"?>
 <layers>
  <layer>
   <name>Outline</name>
   <colourindex>2</colourindex>
   <regex>.*Draw board outline.*</regex>
  </layer>
  <layer>
   <name>Pads</name>
   <colourindex>3</colourindex>
   <regex>.*Draw Parts.*</regex>
  </layer>
.....
```

The elements under layer are as follows:

|Element|Description|
|-------|:-----------|
|name|Name of the layer that will appear in the output|
|colourindex|colour number that will be used for the layer|
|regex|Regular expression which is matched against comments in the gerber file. All records in the gerber file following a match are assigned to this layer|

DXF colour indexes are defined here http://sub-atomic.com/~moses/acadcolors.html
