#include <stdio.h>
#include <bits/stdc++.h>
#include <cstdio>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include<iostream>
#include <cmath>
#include <vector>
#define WINDOW "test"
using namespace std;
using namespace cv;
Mat img(480, 480, CV_8UC3, Scalar(255,255,255));
Mat nextimg ;
Point previousPoint;
int mouse_x;
void On_mouse(int event, int x, int y, int flags, void*);
struct one{
    double vx,vy,px,py;
    int num;
    int hit;
};
vector<struct one>ball;
vector<struct one>ouch;
struct one test;
void free(int i){
    ball[i].vy+=9.8*0.01;
    if(ball[i].py>=430){
        ball[i].vy=-ball[i].vy;
    }
    if(ball[i].px<=20||ball[i].px>=460){
        ball[i].vx=-ball[i].vx;
    }
    ball[i].px+=ball[i].vx*0.01;
    ball[i].py+=ball[i].vy*0.01;
}

int main(){
    Mat img(480, 480, CV_8UC3, Scalar(255,255,255));
    line(img, Point(0,450), Point(480,450), Scalar(255,0,0), 3);
    img.copyTo(nextimg);
    putText(nextimg, std::string("start..."), Point(100,220), 0, 2, Scalar(255,0,255),3);
    imshow(WINDOW, nextimg);
    waitKey(0);
    int i;
    for(i=3;i>0;i--){
        img.copyTo(nextimg);
        putText(nextimg, std::to_string(i), Point(200,220), 0, 2, Scalar(255,0,255),3);
        imshow(WINDOW, nextimg);
        waitKey(500);
    }
    img.copyTo(nextimg);
    putText(nextimg, std::string("go"), Point(190,220), 0, 2, Scalar(255,0,255),3);
    imshow(WINDOW, nextimg);
    waitKey(500);
    //
    test.num=64;
    test.hit=0;
    test.px=240,test.py=50;
    ball.push_back(test);
    int time=0;
    int bsiz,osiz;
    while(1){
        time++;
        img.copyTo(nextimg);
        bsiz=ball.size();
        i=0;
        while(i<bsiz){
            if(ball.size()==0){
                break;
            }
            if(ball[i].num==0){
                waitKey(0);
            }
            if(ball[i].num<=1&&ball[i].hit<1){
                i++;
                continue;
            }
            if((ball[i].num<=1&&ball[i].hit>=1)||ball[i].num==0){
                ball.erase(ball.begin()+i);
                bsiz--;
                continue;
            }

            if(ball[i].hit>=ball[i].num/2){
                test.num=ball[i].num/2;
                test.hit=0;
                test.vx=40;
                test.vy=-ball[i].vy;
                test.py=ball[i].py;
                test.px=ball[i].px;
                ball.push_back(test);
                test.vx=-40;
                ball.push_back(test);
                ball.erase(ball.begin()+i);
                bsiz--;
                continue;
            }
            i++;
        }
        setMouseCallback(WINDOW, On_mouse, 0);
        rectangle(nextimg, Point(mouse_x+15,440), Point(mouse_x-15,410), Scalar(0,0,255), -1);
        for(i=0;i<ball.size();i++){
            free(i);
            circle(nextimg, Point(ball[i].px,ball[i].py), 20, Scalar(ball[i].num*2-1,255,0), -1);
            putText(nextimg, std::to_string(ball[i].num-ball[i].hit), Point(ball[i].px-20,ball[i].py+10), 0, 0.5, Scalar(0,0,0),1);
            if(ball[i].px>=mouse_x-25&&ball[i].px<=mouse_x+25){
                if(ball[i].py>=400&&ball[i].py<=450){
                    putText(nextimg, std::string("you lose!!"), Point(100,220), 0, 2, Scalar(255,0,255),3);
                    imshow(WINDOW, nextimg);
                    waitKey(0);
                    return 0;
                }
            }
        }
        if(ball.size()<=0){
            putText(nextimg, std::string("you win!!"), Point(100,220), 0, 2, Scalar(255,0,255),3);
            imshow(WINDOW, nextimg);
            waitKey(0);
            return 0;
        }
        if(time>=50){
            time=0;
            test.num=1;
            test.px=mouse_x;
            test.py=420;
            test.vx=0;
            test.vy=-200;
            ouch.push_back(test);
        }
        osiz=ouch.size();
        int change=0;
        i=0;

        while(i<osiz){
            if(ouch.size()==0){
                break;
            }
            change=0;
            ouch[i].vy+=9.8*0.01;
            ouch[i].py+=ouch[i].vy*0.01;
            circle(nextimg, Point(ouch[i].px,ouch[i].py), 10, Scalar(0,50,0), -1);
            for(int j=0;j<ball.size();j++){
                if((ouch[i].px-ball[j].px)*(ouch[i].px-ball[j].px)+(ouch[i].py-ball[j].py)*(ouch[i].py-ball[j].py)<=30*30){
                    ouch.erase(ouch.begin()+i);
                    change=1;
                    osiz--;
                    ball[j].hit++;
                    break;
                }
            }
            if(change==1){
                continue;
            }
            if(ouch[i].py<=50){
                ouch.erase(ouch.begin()+i);
                continue;
                osiz--;
            }
            i++;
        }
        if(time%2==0){
            imshow(WINDOW, nextimg);
            waitKey(1);
        }

    }
    printf("hi");
    return 0;
}
void On_mouse(int event, int x, int y, int flags, void*)
{
    Point pt(x, y);
    if(x<10){
        x=10;
    }
    if(x>470){
        x=470;
    }
    mouse_x=x;
}
