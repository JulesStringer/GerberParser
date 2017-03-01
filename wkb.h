#ifndef _WKB_INCLUDED
#define _WKB_INCLUDED 0

enum wkbGeometryType{
    wkbUnknown = 0,
	wkbPoint = 1,
	wkbLineString = 2,
	wkbPolygon = 3,
	wkbMultiPoint = 4,
	wkbMultiLineString = 5,
	wkbMultiPolygon = 6,
	wkbGeometryCollection = 7,
    wkbCircle = 8       //this does not form part of the Open GIS spec and is for internal use only
};

enum wkbByteOrder{
	wkbXDR = 0,
	wkbNDR = 1			// Use this one for Intel Platforms
};

#define WKBPOINT wkbPoint
#define WKBLINESTRING wkbLineString
#define WKBPOLYGON wkbPolygon

#pragma pack(1)

#ifndef GM_POINT_DEFINED 
#define GM_POINT_DEFINED 
typedef struct tagGM_POINT
{
    double x;
    double y;

}   GM_POINT;
#endif

typedef struct tagGM_LinearRing
{
    ULONG       numPoints;
    GM_POINT    Points[1];

}   GM_LinearRing;

typedef struct tagWKB
{
	BYTE        byteOrder;
	ULONG       wkbType;

}   WKB;

typedef struct tagWKBPoint
{
    BYTE        byteOrder;
    ULONG       wkbType;
    GM_POINT    Point;

}   WKBPoint;

typedef struct tagWKBLineString
{
    BYTE        byteOrder;
    ULONG       wkbType;
    ULONG       numpoint;
    GM_POINT    Points[1];

}   WKBLineString;

typedef struct tagWKBPolygon
{
    BYTE            byteOrder;
    ULONG           wkbType;
    ULONG           numRings;
    GM_LinearRing   rings[1];

}   WKBPolygon;

typedef struct tagWKBMultiPoint
{
	BYTE        byteOrder;
	ULONG       wkbType;
	ULONG       num_wkbPoints;
	WKBPoint    WKBPoints[1];

}   WKBMultiPoint;

typedef struct tagWKBMultiLineString
{
	BYTE            byteOrder;
	ULONG           wkbType;
	ULONG           num_wkbLineStrings;
	WKBLineString   WKBLineStrings[1];

}   WKBMultiLineString;

typedef struct tagWKBMultiPolygon
{
	BYTE        byteOrder;
	ULONG       wkbType;
	ULONG       num_wkbPolygons;
	WKBPolygon  WKBPolygons[1];

}   WKBMultiPolygon;

/*
typedef struct tagWKBGeometryCollection WKBGeometryCollection;
typedef struct tagWKBGeometry{
	union{
		WKBPoint				point;
		WKBLineString			linestring;
		WKBPolygon				polygon;
		WKBGeometryCollection	collection;
		WKBMultiPoint			mpoint;
		WKBMultiLineString		mlinestring;
		WKBMultiPolygon			mpolygon;
	}
}WKBGeometry;
*/

typedef struct tagWKBGeometryCollection
{
	BYTE    byteOrder;
	ULONG   wkbType;
	ULONG   num_wkbGeometries;
	WKB     wkb[1];

}   WKBGeometryCollection;

//
//  our own extremely useful specialization of polygon - the rectangle
//
#ifndef WKBRectangle_defined
#define WKBRectangle_defined
typedef struct tagGM_Rectangle
{
    ULONG       numPoints;
    GM_POINT    Points[5];

}   GM_Rectangle;

typedef struct tagWKBRectangle
{
	BYTE            byteOrder;
	ULONG           wkbType;
    ULONG           numRings;
    GM_Rectangle    rings[1];

}   WKBRectangle;
#endif

//the following is for internal use only and is NOT part of the open GIS spec
typedef struct tagWKBCircle
{
    BYTE            byteOrder;
    ULONG           wkbType;
    GM_POINT        Centre;
    double          Radius;
} WKBCircle;

#pragma pack()

#endif _WKB_INCLUDED
