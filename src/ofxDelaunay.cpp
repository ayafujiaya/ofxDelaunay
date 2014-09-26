/*
 *  ofxDelaunay.h
 *
 *  Created by Pat Long on 29/10/09.
 *  Copyright 2009 Tangible Interaction. All rights reserved.
 *
 *  Some parts based on demo code by Gilles Dumoulin. 
 *  Source: http://local.wasp.uwa.edu.au/~pbourke/papers/triangulate/cpp.zip
 *
 *	Cleaned up by Lukasz Karluk, 19/05/2010
 *
 *  Refactored for 0071 + ofMesh by James George on 21/06/2012
 */

#include "ofxDelaunay.h"



void ofxDelaunay::reset(){
    vertices.clear();
    triangles.clear();
	triangleMesh.clear();
}

int ofxDelaunay::addPoint( const ofPoint& point ){
	return addPoint( point.x, point.y, point.z );
}

int ofxDelaunay::addPoint( float x, float y, float z ){
    XYZ v;
    v.x = x;
    v.y = y;
    v.z = z;
    vertices.push_back(v);
	return vertices.size();
}

int ofxDelaunay::addPoints( vector<ofPoint>& points ){
	for(int i = 0; i < points.size(); i++){
        addPoint( points[i] );
    }
    return vertices.size();
}

int ofxDelaunay::triangulate(){
    
    if(vertices.size() < 3){
        return NULL;
    }
    
    int nv = vertices.size();
    //add 3 emptly slots, required by the Triangulate call
    vertices.push_back(XYZ());
    vertices.push_back(XYZ());
    vertices.push_back(XYZ());
    
    //allocate space for triangle indices
    triangles.resize(3*nv);
    
	int ntri;
	qsort( &vertices[0], vertices.size()-3, sizeof( XYZ ), XYZCompare );
	Triangulate( nv, &vertices[0], &triangles[0], ntri );
	
	// copy triangle data to ofxDelaunayTriangle.
	triangleMesh.clear();
    
    //copy vertices
	for (int i = 0; i < nv; i++){
        triangleMesh.addVertex(ofVec3f(vertices[i].x,vertices[i].y,vertices[i].z));
        
    }
    
    //copy triagles

	for(int i = 0; i < ntri; i++){
        float fColor = ofMap(i, 0, ntri, 0, 1.0);
		triangleMesh.addIndex(triangles[ i ].p1);
		triangleMesh.addIndex(triangles[ i ].p2);
		triangleMesh.addIndex(triangles[ i ].p3);
	}
	
    //erase the last three triangles
    vertices.erase(vertices.end()-1);
    vertices.erase(vertices.end()-1);
    vertices.erase(vertices.end()-1);
	return ntri;
}

int ofxDelaunay::triangulate(vector<ofFloatColor> _fColors, float _width, float _height){
    

    
    if(vertices.size() < 3){
        return NULL;
    }
    
    int nv = vertices.size();
    //add 3 emptly slots, required by the Triangulate call
    vertices.push_back(XYZ());
    vertices.push_back(XYZ());
    vertices.push_back(XYZ());
    
    //allocate space for triangle indices
    triangles.resize(3*nv);
    
	int ntri;
	qsort( &vertices[0], vertices.size()-3, sizeof( XYZ ), XYZCompare );
	Triangulate( nv, &vertices[0], &triangles[0], ntri );
	
	// copy triangle data to ofxDelaunayTriangle.
	triangleMesh.clear();
    
    //copy vertices
	for (int i = 0; i < nv; i++){
        triangleMesh.addVertex(ofVec3f(vertices[i].x,vertices[i].y,vertices[i].z));
        
    }
    
    //copy triagles
    
	for(int i = 0; i < ntri; i++){
        float fColor = ofMap(i, 0, ntri, 0, 1.0);
		triangleMesh.addIndex(triangles[ i ].p1);
		triangleMesh.addIndex(triangles[ i ].p2);
		triangleMesh.addIndex(triangles[ i ].p3);
	}
    
    for(int i=0; i < triangleMesh.getNumVertices();i++) {
        triangleMesh.addColor(ofColor(0,0,0));
    }
    
    /* 頂点部分の色を計算 ３つの頂点の純真部分の色を三角形の色としてさいようしたいが未実装 */
    for(int i=0;i<triangleMesh.getNumIndices() / 3;i+=1) {
        ofVec3f v1 = triangleMesh.getVertex(triangleMesh.getIndex(i*3));
        ofVec3f v2 = triangleMesh.getVertex(triangleMesh.getIndex(i*3+1));
        ofVec3f v3 = triangleMesh.getVertex(triangleMesh.getIndex(i*3+2));
        ofVec3f vCenter = (v1 + v2 + v3) / 3.0;
        
        vCenter.x = ofClamp(vCenter.x, 0, _width);
        vCenter.y = ofClamp(vCenter.y, 0, _height);
        
        ofPoint point;
        point.x = vCenter.x;
        point.y = vCenter.y;
        //triCenter.push_back(point);
        
        ofColor c = _fColors[v1.x + _width * v1.y];
        
        triangleMesh.setColor(triangleMesh.getIndex(i*3),c);
        triangleMesh.setColor(triangleMesh.getIndex(i*3+1),c);
        triangleMesh.setColor(triangleMesh.getIndex(i*3+2),c);
    }
    
	
    //erase the last three triangles
    vertices.erase(vertices.end()-1);
    vertices.erase(vertices.end()-1);
    vertices.erase(vertices.end()-1);
	return ntri;
}

void ofxDelaunay::draw(){
    
    //postFx.begin();
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glShadeModel(GL_FLAT);
	glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);
    
	if(ofGetStyle().bFill){
	    triangleMesh.draw();
    } else {
    	triangleMesh.drawWireframe();    
    }
    glShadeModel(GL_SMOOTH);
	glPopAttrib();
    //postFx.end();
}


