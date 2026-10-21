#include<iostream>
#include<opencv2/opencv.hpp>
using namespace std;
using namespace cv;
int main(){
    VideoCapture cap("test.avi");
    
    if(!cap.isOpened()) {
        cout<<"Errors:the video failed to be opened.Please check the filename and the route."<<endl;
        return -1;
    }
    Mat frame;
   
    while(true){
        cap>>frame;
        if(frame.empty()) break;
    Mat frameHSV;
    Mat mask;


     cvtColor(frame,frameHSV,COLOR_BGR2HSV);
     Scalar lower_red(0,80,80);
     Scalar upper_red(10,255,255);
      
    inRange(frameHSV,lower_red,upper_red,mask);
    
    Mat kernel = getStructuringElement(MORPH_RECT,Size(7,7));
    
        morphologyEx(mask,mask,MORPH_CLOSE,kernel);


vector<vector<Point>> contours;
        findContours(mask,contours,RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);
vector<RotatedRect>valid_light_bars; 


vector<Point2f> topPoints;
vector<Point2f> bottomPoints;


for(size_t i = 0;i<contours.size();i++){
    double area = contourArea(contours[i]);

    if(area < 20)  continue;

RotatedRect rRect = minAreaRect(contours[i]);

valid_light_bars.push_back(rRect);
}

if(valid_light_bars.size() == 2){

for(int i = 0;i< 2;i++){
Point2f pts[4];
valid_light_bars[i].points(pts);
Point2f top_pt = pts[0],bottom_pt = pts[0];
for(int k = 1;k<4;k++){
    if(pts[k].y<top_pt.y) top_pt = pts[k];
    if(pts[k].y>bottom_pt.y) bottom_pt = pts[k];
}
Point2f ntop_pt;
Point2f nbottom_pt;
   ntop_pt.y = top_pt.y;
   nbottom_pt.y = bottom_pt.y;
   ntop_pt.x = valid_light_bars[i].center.x;
   nbottom_pt.x = valid_light_bars[i].center.x;

topPoints.push_back(ntop_pt);
bottomPoints.push_back(nbottom_pt);
}


Point2f pts1[4],pts2[4];
vector<Point2f> allpoints;
    valid_light_bars[0].points(pts1);
    valid_light_bars[1].points(pts2);
for(int k = 0;k<4;k++){
    allpoints.push_back(pts1[k]);
    allpoints.push_back(pts2[k]);
}

RotatedRect big_rect = minAreaRect(allpoints);

vector<Point2f> Final2;
Point2f finalpoints[4];

for(int i = 0;i<2;i++){
    Final2.push_back(topPoints[i]);
}
for(int i = 1;i>=0;i--){
    Final2.push_back(bottomPoints[i]);
}




big_rect.points(finalpoints);
        
for(int i = 0;i<4;i++){
    //line(frame,finalpoints[i],finalpoints[(i+1) % 4],Scalar(0,255,0),2);
     line(frame,Final2[i],Final2[(i+1) % 4],Scalar(0,255,0),2);
}


}
        imshow("Video playback",frame);
        if(waitKey(30) == 27)  break;

    }
cap.release();
destroyAllWindows();

return 0;

}