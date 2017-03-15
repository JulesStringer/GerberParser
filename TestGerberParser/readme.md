This folder contains a test program for GerberParser.

It is a command line application, which produces a DXF output from one or more gerbers.

#Command Line arguments

Each argument has a tag beginning with - which may be followed by one or more further arguments.

||Tag||Description||
||-folder <folder>||Specifies a folder which must contain files top_paste_mask.grb and drill_file.drl. An output is produced based on these files.||
||-params <parameterfile>||Specifies a parameter file which controls the output. If this is specified the -folder and -gerber arguments should not be.||

