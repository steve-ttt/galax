/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: steve
 *
 * Created on 20 August 2019, 1:42 PM
 */
#include <cstdlib>
#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#define OLC_PGE_GRAPHICS2D
#include "olcPGEX_Graphics2D.h"
#include <math.h>

#define PI 3.14159265

using namespace std;


int SCREENWIDTH = 600;
int SCREENHEIGHT = 800;

                            /*
                             *  tan(theta) = opposite / adjacent
                             *  y = 4; opposite
                             *  x = 3; adjacent 
                                result = atan2 (y,x) ; //* 180 / PI;  ### ( * 180/ PI to convert rad to deg ) 
                                std::cout << result << "\n";
                                float opp = tan(0.927295) * 3;
                                std::cout << opp <<  "\n";
                             */


	// Override base class with your custom functionality
class Galax : public olc::PixelGameEngine
    {
	public:
		Galax()
		{
			sAppName = "Galax";
		}
	public:
            
                float fTotalTime = 0.0f;
                float bulletSpeed = 250.0;
                float fBadGuySpeed = 70.0f; 
                olc::Sprite *sprShip;
                olc::Sprite *sprBullet;
                olc::Sprite *sprBad1;
                olc::Sprite *sprBad2;
                olc::Sprite *sprBg;
                olc::Sprite *sprTitle;
                int nScore = 0;
                int nPlayerHP = 3;
                bool gameOver = true;
                
        	struct sSpaceObject
                {
                    int nHitPoints;
                    float x;
                    float y;
                    float dx;
                    float dy;
                    float angle;
                };
                vector<sSpaceObject> vecBullets;
                vector<sSpaceObject> vecBadGuys; 
                vector<sSpaceObject> badGuy2 ;
                
                
                bool recToPointCollision(float x, float y, float w, float h, float px, float py) {
                    if ( x <= px && px <= x + w && y  <= py && py <= y + h)
                    {
                        return true;
                    } else 
                    {
                        return false;
                    }
                }
                        
                bool RectRectCollision (float boxAx, float boxAy, float boxAwidth, float boxAheight,
					  float boxBx, float boxBy, float boxBwidth, float boxBheight ) 
                {
		  if( (boxAx <= boxBx + boxBwidth && boxAx + boxAwidth  >= boxBx) &&
		      (boxAy <= boxBy + boxBwidth && boxAy + boxAwidth  >= boxBy)) 
                  {
		    return true;
		  } else {
		    return false;
		  }
		  
		}        
                        
		bool OnUserCreate() override
		{
                    // Called once at the start, so create things here		
                    sprShip = new olc::Sprite("Assets/ship.png");
                    sprBullet = new olc::Sprite("Assets/bullet1.png");
                    sprBad1 = new olc::Sprite("Assets/bad1.png");
                    sprBad2 = new olc::Sprite("Assets/bad2.png");
                    sprBg = new olc::Sprite("Assets/bg.png");
                    sprTitle = new olc::Sprite("Assets/galax.png");
                    nScore = 0;
                    nPlayerHP = 3;
                    vecBadGuys.clear() ;
                    badGuy2.clear();
                    vecBullets.clear();
                    
                    
                    for (int i =0; i <= 5; i++) {
                        vecBadGuys.push_back({1, (float)(rand() % ScreenWidth()) - 60, (float)(rand() % ScreenHeight() /2 * -1), fBadGuySpeed * sinf(0.0f), fBadGuySpeed * cosf(0.0f) , 0.0f});
                    }
                    for (int i =0; i <= 1; i++) {
                        badGuy2.push_back({3, (float)(rand() % ScreenWidth()), (float)(rand() % ScreenHeight() + 100) * -1 , 0, 0, 0});
                    }
                    return true;
		}
                
                
		bool OnUserUpdate(float fElapsedTime) override
		{
                    SetPixelMode(olc::Pixel::NORMAL);
                    DrawSprite(0, 0, sprBg); //background art
                    SetPixelMode(olc::Pixel::ALPHA);
                    DrawSprite(100, 300, sprTitle);
                    SetPixelMode(olc::Pixel::NORMAL);  
                    DrawString(2, 2, "  SCORE: " + to_string(nScore));
                    DrawString(12, 24, "  press P to play " );
                    DrawString(12, 38, "  press Q to quit " );
                    DrawString(12, 50, "  ... " );
                    
                    if (GetKey(olc::Key::P).bPressed) {       
                             gameOver = false;
                             OnUserCreate(); 
                    }
                    if (GetKey(olc::Key::ESCAPE).bPressed || GetKey(olc::Key::Q).bPressed && gameOver) {  
                            return false;
                    }
                  
                  while(!gameOver) {
                    // called once per frame, draws random coloured pixels
                    SetPixelMode(olc::Pixel::NORMAL);
                    //Clear(olc::VERY_DARK_BLUE);
                    DrawSprite(0, 0, sprBg); //background art

                    DrawString(2, 2, "  SCORE: " + to_string(nScore));    
                    DrawString(2, 12, "  Player HP: " + to_string(nPlayerHP));   
                       

                    float mx = (float)GetMouseX();
                    float my = (float)GetMouseY();
                    if(nPlayerHP <= -1) gameOver = true;

                    // Draw Spaceship Sprite using extension, first create a transformation stack
                    olc::GFX2D::Transform2D t1;
                    // translate sprShipite 
                    t1.Translate(-60/2, -85/2);
                    t1.Translate(mx, my);
                    SetPixelMode(olc::Pixel::ALPHA);
                    // Use extension to draw sprShip with transform applied
                    olc::GFX2D::DrawSprite(sprShip, t1);


                    if (GetKey(olc::Key::SPACE).bReleased  || GetKey(olc::Key::CTRL).bReleased || GetMouse(0).bPressed)
                        vecBullets.push_back({ 0, mx -11, my - 45, bulletSpeed * sinf(0.0f), - bulletSpeed * cosf(0.0f), 0.0f });
                    if (GetKey(olc::Key::ESCAPE).bPressed || GetKey(olc::Key::Q).bPressed ) 
                    {
                        nPlayerHP = -1;  //press esc or q to exit
                    }




                    //update bullets
                    for (auto &b : vecBullets)
                    {
                        b.x += b.dx * fElapsedTime;
                        b.y += b.dy * fElapsedTime;
                        b.angle -= 1.0f * fElapsedTime;

                        for (auto &bad2 : badGuy2)  // check bullet collisions
                        {
                            if(recToPointCollision(bad2.x - 65/2, bad2.y - 89/2, 65.0f, 89.0f, b.x, b.y))
                            {
                                if (bad2.nHitPoints <= 1) 
                                {
                                    b.y = -40.0f;
                                    bad2.x = (float)(rand() % ScreenWidth()) - 60;
                                    bad2.y = (float)(rand() % ScreenHeight() * -1);
                                    bad2.nHitPoints = 3;
                                    nScore += 50 ;                            
                                } else {
                                    b.y = -40.0f;
                                    nScore += 10;
                                    bad2.nHitPoints-- ;
                                }
                            }
                        }
                        for (auto &bad : vecBadGuys)   // check bullet collisions
                        {                       
                            if(recToPointCollision(bad.x , bad.y , 63.0f, 34.0f, b.x, b.y))  
                            {
                                b.y = -40.0f; //send the bullet off the screen to be cleaned up later
                                bad.x = (float)(rand() % ScreenWidth()) - 60;       // reset baddie x pos to starting 
                                bad.y = (float)(rand() % ScreenHeight() /2 * -1);   // reset baddie y pos to starting 
                                nScore += 10 ;
                            }

                        }
                        // Check bullet collision with player 
                        if(recToPointCollision(mx , my , 60.0f, 85.0f, b.x+15.0f/2, b.y+15.0f/2)) 
                        {
                            b.y = -40.0f; //send the bullet off the screen to be cleaned up later
                            nPlayerHP-- ;
                        }
                        
                    }

                    //draw bullets
                    for (auto b : vecBullets)
                        //Draw(b.x, b.y);
                        DrawSprite(b.x, b.y, sprBullet);


                    //update baddies
                    for (auto &bad : vecBadGuys)
                    {
                        // if he goes off screen reset his position
                        if ( bad.y >= ScreenHeight() ){
                            bad.y = (float)(rand() % ScreenHeight() /2 * -1);
                            bad.x = (float)(rand() % ScreenWidth() - 60);
                        }
                        // check Player collision
                        if(RectRectCollision(bad.x , bad.y , 60.0f, 34.0f, mx , my , 60.0f, 85.0f))  
                        {
                            nPlayerHP-- ;
                            bad.x = (float)(rand() % ScreenWidth()) - 60;       // reset baddie x pos to starting 
                            bad.y = (float)(rand() % ScreenHeight() /2 * -1);   // reset baddie y pos to starting 
                            nScore += 10 ;
                        }
                        // if he gets to X pos of 30 have him shoot a bullet 
                        if(bad.y >= 30.0f && bad.y <= 33.0f ) {
                            vecBullets.push_back({ 0, bad.x +30.0f-7.5f, bad.y + 34.0f , bulletSpeed * sinf(3.1415f), - bulletSpeed * cosf(3.1415f), 0.0f });
                        }

                        bad.x += bad.dx * fElapsedTime;
                        bad.y += bad.dy * fElapsedTime;
                        bad.angle -= 1.0f * fElapsedTime;

                    }    

                    //update badGuy2
                    for (auto &bad2 : badGuy2)
                    {
                        // check Player collision
                        if(RectRectCollision(bad2.x , bad2.y, 65.0f, 89.0f, mx , my , 60.0f, 85.0f))  
                        {
                            nPlayerHP-- ;
                            bad2.x = (float)(rand() % ScreenWidth()) - 60;       // reset baddie x pos to starting 
                            bad2.y = (float)(rand() % ScreenHeight() * -1);   // reset baddie y pos to starting 
                            nScore += 10 ;
                        }


                        olc::GFX2D::Transform2D bb1;    
                        bb1.Translate(-65/2, -89/2);     // Translate sprite so center of image is at 0,0
                        bad2.angle = atan2( (bad2.x - mx) , (bad2.y - my) )   ; //angle to player in radians
                        bad2.dx = -(fBadGuySpeed * 1.5f) * sinf(bad2.angle) ;         // fBadGuySpeed * 1.5f 'cause he should be faster than the lower ranks
                        bad2.dy = -(fBadGuySpeed * 1.5f) * cosf(bad2.angle) ;
                        bad2.x +=  bad2.dx * fElapsedTime;
                        bad2.y +=  bad2.dy * fElapsedTime;
                        bb1.Rotate(bad2.angle)    ;
                        bb1.Translate(bad2.x, bad2.y);
                        SetPixelMode(olc::Pixel::ALPHA);
                        olc::GFX2D::DrawSprite(sprBad2, bb1);
                        SetPixelMode(olc::Pixel::NORMAL);
                        
                        // give the bad2 guy a shot
                        if(bad2.y >= 40.0f && bad2.y <= 42.0f ) {
                            vecBullets.push_back({ 0, bad2.x , bad2.y + 50.0f , bulletSpeed * sinf(-bad2.angle), - bulletSpeed * cosf(bad2.angle), bad2.angle });
                        }
                        if(bad2.y >= 400.0f && bad2.y <= 402.0f ) {
                            vecBullets.push_back({ 0, bad2.x +65.0f/2 , bad2.y + 89.0f , bulletSpeed * sinf(-bad2.angle), - bulletSpeed * cosf(bad2.angle), bad2.angle });
                        }
                        
                    }

                    //draw BadGuys
                    for (auto bad : vecBadGuys)
                    {
                        //FillRect(bad.x, bad.y, 20, 20, olc::DARK_MAGENTA);
                        SetPixelMode(olc::Pixel::ALPHA);
                        DrawSprite(bad.x, bad.y, sprBad1);
                        SetPixelMode(olc::Pixel::NORMAL);
                    }



                     //erase bullets off screen       
                    if (vecBullets.size() > 0)
                    {
                            auto i = remove_if(vecBullets.begin(), vecBullets.end(), [&](sSpaceObject o) { return (o.x < 1 || o.y < 1 || o.x >= ScreenWidth() - 1 || o.y >= ScreenHeight() - 1); });
                            if (i != vecBullets.end())
                                    vecBullets.erase(i);
                    }




                            return true;
                    }
                }
	};
        
        
int main()
    {
		Galax demo;
		if (demo.Construct(SCREENWIDTH, SCREENHEIGHT, 1, 1))
			demo.Start();
                
		return 0;
	}