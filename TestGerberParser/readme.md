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
|colourindex|colour number that will be used for the layer, these are DXF colour indexes as defined here http://sub-atomic.com/~moses/acadcolors.html|
|regex|Regular expression which is matched against comments in the gerber file. All records in the gerber file following a match are assigned to this layer|
