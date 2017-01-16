//Can You Escape?
//Use the clues to get out of the room!
//by Carolyn Bergdolt, inspiration credit to Kaarel Kirisipuu

#include <stdio.h>
#include "gfx5.h"
#include <unistd.h>

struct Room {		//holds the status of each of the clues
	int key;	//still hidden = 0
	int colorcard;	//found = 1 
	int handle;	//found and in toolbox = 2
};			//found and selected = 3
			//used = 4

void lightswitch();
void light(int, int, int);
void room(struct Room*);
void baiscRoom(struct Room*);
void keyhole(int, int);
void handle(int, int, struct Room*);
void colorcard(int, int);
void key(int, int);
void backarrow();
void window(struct Room*);
void cabinet(struct Room*);
void rug(struct Room*);
void door(struct Room*);
void openDoor();

int main() {
	int w = 800, h = 600;
	gfx_open(w, h, "Can You Escape?");
	gfx_clear();
	lightswitch();
	struct Room roomOne = {0, 0, 0};
	struct Room* r = &roomOne;
	room(r);
}

void lightswitch() {
	gfx_color(203, 156, 0);
	gfx_fill_rectangle(20, 260, 20, 40);
	gfx_color(255, 255, 255);
	gfx_text(40, 40, "Click the lightswitch to turn on the light!");
	int x, y;
	char c;
	while (1) {
		c = gfx_wait();
		if (c == 1) {
			x = gfx_xpos();
			y = gfx_ypos();
			if ((x >= 20 && x <= 40) && (y >= 260 && y <= 300)) {
				gfx_clear();
				light(900, 0, 0);
				return;
			}		
		}
	}
}

void light(int r, int rg, int b){
	//base case
	if (r < 12 || rg > 255) return;
	//recurse
	light(r-7, rg+2, b+1);
	//draw circle
	gfx_color(rg, rg, b);
	gfx_fill_circle(400, 0, r);
	usleep(5000);
	gfx_flush();
}

void basicRoom(struct Room* r) {
	XPoint walls[] = {{40,560},{40,120},{160,120},{480,120},{600,120},
				{600,560},{480,360},{160,360}};
	XPoint floor[] = {{40,560},{160,360},{480,360},{600,560}};
	XPoint rug[] = {{200,520},{280,400},{440,400},{520,520},{200,520}};
	XPoint cabinet[] = {{60,240},{140,200},{200,200},{200,400},
				{120,520},{60,520}};
	XPoint door[] = {{260,360},{260,160},{360,160},{360,360}};
	XPoint opendoor[] = {{260,360},{260,160},{235,120},{235,400},{260,360}};
	XPoint window[] = {{500,180},{520,190},{520,290},{580,340},{580,380},{500,300}};
	XPoint windowlines[] = {{520,190},{500,180},{500,300},{580,380},
				{580,220},{520,190},{520,290},{580,340}};
	XPoint cabinetlines[] = {{200,200},{140,200},{60,240},{60,520},{120,520},
				{200,400},{200,200},{120,240},{120,520},
				{160,460},{160,220}};
	//background
	gfx_color(248,255,174);
	gfx_fill_polygon(walls, 8);
	gfx_color(94,56,0);
	gfx_fill_polygon(floor, 4);
	//cabinet, door, rug window
	gfx_color(181,108,0);
	gfx_fill_polygon(cabinet, 6);
	gfx_color(122,123,141);
	gfx_fill_polygon(door, 4);
	keyhole(340, 260);
	gfx_color(38,127,0);
	gfx_fill_polygon(rug, 5);
	gfx_color(228,236,160);
	gfx_fill_polygon(window, 6);
	handle(160, 340, r);
	//outlines
	gfx_color(0,0,0);
	gfx_line(160,120,160,200);//wall lines
	gfx_line(480,120,480,360);
	gfx_line(200,360,480,360);//floor lines
	gfx_line(480,360,600,560);
	gfx_line(40,560,64,520);
	gfx_polygon(door, 4);
	gfx_polygon(windowlines, 8);
	gfx_line(500,300,520,290);//finish window
	gfx_polygon(cabinetlines, 11);
	gfx_line(60,240,120,240);//finish cabinet
	gfx_polygon(rug, 5);
	//open door if it has been unlocked
	if (r->key == 4){
		gfx_color(28,28,28);
		gfx_fill_polygon(door, 4);
		gfx_color(122,123,141);
		gfx_fill_polygon(opendoor, 5);
		gfx_color(0,0,0);
		gfx_polygon(opendoor, 5);
		gfx_color(255,255,255);
		gfx_changefont("9x15bold");
		gfx_text(40, 40, "You did it!");
		gfx_text(40, 60, "Congratulations!");
	}
	return;
}

void room (struct Room* r) {
	gfx_clear();
	gfx_color(255, 255, 255);
	gfx_text(40, 40, "Your main objective is to escape from this room.");
	gfx_text(40, 55, "Look around the room for clues to help you escape.");
	gfx_text(40, 70, "Press 'q' to exit.");
	gfx_line(640, 0, 640, 600);
	gfx_text(680, 80, "TOOLBOX");
	char c = '0';
	int x, y;
	while (c != 'q'){
		basicRoom(r);
		c = gfx_wait();
		if (c == 1) {
			x = gfx_xpos();
			y = gfx_ypos();
			if ((x >= 500 && x <= 580) && (y >= 180 && y <= 380)) window(r);
			else if ((x >= 60 && x <= 200) && (y >= 200 && y <= 460)) cabinet(r);
			else if ((x >= 200 && x <= 520) && (y >= 400 && y <= 520)) rug(r);
			else if ((x >= 260 && x <= 360) && (y >= 160 && y <= 360)) {
				if (r->key == 4) openDoor();
				else door(r);
			}
		}
	}
	return;
}

void keyhole(int x, int y){
	XPoint triangle[] = {{x, y-3},{x+4, y+7},{x-4, y+7}};
	gfx_color(189,189,0);
	gfx_fill_circle(x, y, 10);
	gfx_color(0,0,0);
	gfx_fill_circle(x, y-3, 4);
	gfx_fill_polygon(triangle, 3);
}

void handle(int x, int y, struct Room* r) {
	gfx_color(189,189,0);
	gfx_arc(x, y-10, 20, 20, 0, 90);
	gfx_arc(x+1, y-9, 18, 18, 0, 90);
	gfx_arc(x, y-10, 20, 20, 0, -90);
	gfx_arc(x+1, y-9, 18, 18, 0, -90);
	if (r->handle == 0) {
		gfx_arc(550, 335, 20, 20, 0, 135);//handle on windowsill
		gfx_arc(551, 336, 18, 18, 0, 135);
		gfx_arc(550, 335, 20, 20, 0, -45);
		gfx_arc(551, 336, 18, 18, 0, -45);
		gfx_color(0,0,0);
		gfx_line(x-11, y-3, x-9, y-3);//first hole
		gfx_line(x-10, y-4, x-10, y-2);
		gfx_line(x-11, y+17, x-9, y+17);//second hole
		gfx_line(x-10, y+18, x-10, y+16);
	}
	else if (r->handle == 2 || r->handle == 3) {
		gfx_fill_arc(680, 160, 80, 80, 0, 90);//handle in toolbox
		gfx_fill_arc(680, 160, 80, 80, 0, -90);
		gfx_color(0,0,0);
		gfx_fill_arc(684, 164, 72, 72, 0, 90);
		gfx_fill_arc(684, 164, 72, 72, 0, -90);
		gfx_line(x-11, y-3, x-9, y-3);//first hole
		gfx_line(x-10, y-4, x-10, y-2);
		gfx_line(x-11, y+17, x-9, y+17);//second hole
		gfx_line(x-10, y+18, x-10, y+16);
	}
	else if (r->handle == 4) {
		gfx_arc(x-20, y-3, 20, 20, 0, 90); //handle on cabinet
		gfx_arc(x-19, y-2, 18, 18, 0, 90);
		gfx_arc(x-20, y-3, 20, 20, 0, -90);
		gfx_arc(x-19, y-2, 18, 18, 0, -90);
	}
	return;
}

void backarrow() {
	gfx_color(19,0,189);
	XPoint arrow[] = {{90,500},{100,490},{100,495},{120,495},{120,505},
				{100,505},{100,510}};
	gfx_fill_polygon(arrow, 7);
	return;
}

void window(struct Room* r) {
	XPoint background[] = {{80,160},{560,160},{560,520},{80,520}};
	XPoint edges[] = {{120,480},{120,160},{160,160},{160,360},{480,360},
				{480,160},{520,160},{520,480}};
	XPoint edgelines[] = {{120,480},{120,160},{160,160},{160,360},{120,480},
				{520,480},{480,360},{480,160},{520,160},{520,480}};
	gfx_cleararea(40, 120, 560, 440);
	gfx_color(248,255,174);
	gfx_fill_polygon(background, 4);
	gfx_color(228,236,160);
	gfx_fill_polygon(edges, 8);
	gfx_color(0,0,0);
	gfx_polygon(edgelines, 10);
	gfx_line(160,360,480,360);
	backarrow();
	int exit = 0, x, y;
	char c;
	while (exit == 0) {
		if (r->handle == 0) {
			gfx_color(189,189,0);//draw handle on windowsill
			gfx_fill_arc(300, 380, 120, 120, 0, 180);
			gfx_color(228,236,160);
			gfx_fill_arc(304, 384, 112, 112, 0, 180);
			gfx_cleararea(0, 0, 639, 100);
			gfx_text(40, 40, "You found a handle!");
			gfx_text(40, 55, "Click the handle to put it in your toolbox.");
			c = gfx_wait();
			if (c == 1) {
				x = gfx_xpos();
				y = gfx_ypos();		// vvv handle click
				if ((x >= 300 && x <= 420) && (y >= 380 && y <= 440)) {
					gfx_fill_rectangle(300, 380, 120, 60);
					r->handle = 2;
					//put handle in toolbox
					gfx_color(189,189,0);
					gfx_fill_arc(680, 160, 80, 80, 0, 90);
					gfx_fill_arc(680, 160, 80, 80, 0, -90);
					gfx_color(0,0,0);
					gfx_fill_arc(684, 164, 72, 72, 0, 90);
					gfx_fill_arc(684, 164, 72, 72, 0, -90);
				}
			}
		}
		c = gfx_wait();
		if (c == 1){
			x = gfx_xpos();
			y = gfx_ypos();		// vvv back arrow click
			if ((x >= 80 && x <= 120) && (y >= 480 && y <= 520)) {
				basicRoom(r);
				exit = 1;
			}
		}
	}
	gfx_cleararea(0, 0, 639, 100);
	return;
}

void cabinet (struct Room* r) {
	gfx_cleararea(40, 120, 560, 440);
	gfx_color(181,108,0);
	gfx_fill_rectangle(80, 160, 480, 360);
	gfx_color(189,189,0);
	gfx_line(380, 300, 380, 340);
	gfx_line(381, 300, 381, 340);
	gfx_color(0,0,0);
	gfx_line(360, 160, 360, 520);
	backarrow();
	char c;
	int exit = 0, x, y;
	while (exit == 0) {
		if (r->handle == 0){ //handle hasn't been found yet
			gfx_cleararea(0, 0, 639, 100);
			gfx_color(255,255,255);
			gfx_text(40, 40, "You must find a handle to open the cabinet.");
			gfx_color(0,0,0);
			gfx_fill_circle(340, 300, 2);
			gfx_fill_circle(340, 340, 2);
		}
		else if (r->handle == 2) {
			gfx_color(0,0,0);
			gfx_fill_circle(340, 300, 1);
			gfx_fill_circle(340, 340, 1);
			c = gfx_wait();
			if (c == 1) {
				x = gfx_xpos();
				y = gfx_ypos();		// vvv select handle
				if ((x >= 720 && x <= 760) && (y >= 160 && y <= 240)) {
					gfx_color(0,242,16);
					gfx_rectangle(710, 150, 60, 100);
					r->handle = 3;
				}
			}
		}
		else if (r->handle == 3) {
			gfx_color(0,0,0);
			gfx_fill_circle(340, 300, 1);
			gfx_fill_circle(340, 340, 1);
			c = gfx_wait();
			if (c == 1) {
				x = gfx_xpos();
				y = gfx_ypos();		// vvv click to place handle
				if ((x >= 320 && x <= 360) && (y >= 280 && y <= 360)) { 
					// vvv remove handle from toolbox
					gfx_cleararea(710, 150, 70, 110);
					gfx_color(181,108,0);
					gfx_fill_rectangle(320, 280, 40, 80);//cover holes
					gfx_color(189,189,0);
					gfx_line(340, 300, 340, 340);//put handle on cabinet
					gfx_line(339, 300, 339, 340);
					r->handle = 4;
				}
			}
		}
		else if (r->handle == 4 && r->colorcard == 0) { //color card not showing
			gfx_cleararea(0, 0, 639, 100);
			gfx_color(189,189,0);
			gfx_line(340, 300, 340, 340); //put handle on cabinet
			gfx_line(339, 300, 339, 340);
			gfx_color(255,255,255);
			gfx_text(40, 40, "Click the handle to open the cabinet.");
			c = gfx_wait();
			if (c == 1) {
				x = gfx_xpos();
				y = gfx_ypos();		// vvv click handle
				if ((x >= 320 && x <= 360) && (y >= 280 && y <= 360)) {
					gfx_color(145,87,0);
					gfx_fill_rectangle(80, 160, 280, 320);
					colorcard(220, 320);
					r->colorcard = 1;
				}
			}
		}
		else if (r->handle == 4 && r->colorcard == 1){ //color card showing
			gfx_cleararea(0, 0, 639, 100);
			gfx_color(255,255,255);
			gfx_text(40, 40, "You found a color card!");
			gfx_text(40, 55, "Click the color card to add it to your toolbox.");
			c = gfx_wait();
			if (c == 1){
				x =  gfx_xpos();
				y = gfx_ypos();		// vvv click color card	
				if ((x >= 160 && x <= 280) && (y >= 240 && y <= 400)) {
					gfx_color(145,87,0);
					gfx_fill_rectangle(80, 160, 280, 320);
					colorcard(720, 340);
					r->colorcard = 2;
				}
			}
		}
		else if (r->handle == 4 && r->colorcard == 2){//cabinet revisited
			gfx_color(145,87,0); //open cabinet
			gfx_fill_rectangle(80, 160, 280, 320);
		}
		c = gfx_wait();
		if (c == 1){
			x = gfx_xpos();
			y = gfx_ypos();		//click back arrow
			if ((x >= 80 && x <= 120) && (y >= 480 && y <= 520)) {
				//if colorcard wasn't actually picked up
				if (r->colorcard == 1) r->colorcard = 0;
				gfx_cleararea(0, 0, 639, 100);
				basicRoom(r);
				exit = 1;
			}
		}
	}
	return;
}

void rug(struct Room* r) {
	XPoint rug[] = {{360,160},{560,160},{560,480}};
	gfx_cleararea(40, 120, 560, 440);
	gfx_color(94,56,0);
	gfx_fill_rectangle(80, 160, 480, 360);
	gfx_color(33,110,0);
	gfx_fill_polygon(rug, 3);
	gfx_color(0,0,0);
	gfx_fill_rectangle(140, 220, 200, 200);
	backarrow();
	char c;
	int exit = 0, x, y;
	int bl = 1, ye = 2, gr = 3, re = 4;
	//variable name is the color that the box SHOULD be, number is the color the box IS
	//red = 1, green = 2, blue = 3, yellow = 4
	gfx_color(255,0,0);
	gfx_fill_rectangle(160, 330, 70, 70);
	gfx_color(0,255,0);
	gfx_fill_rectangle(250, 330, 70, 70);
	gfx_color(0,0,255);
	gfx_fill_rectangle(250, 240, 70, 70);
	gfx_color(255,230,0);
	gfx_fill_rectangle(160, 240, 70, 70);
	while (exit == 0) {
		if (r->key == 0){
			c = gfx_wait();
			if (c == 1) {
				x = gfx_xpos();
				y = gfx_ypos();		// vvv re box
				if ((x >= 160 && x <=230) && (y >= 240 && y <= 310)) {
					re++;
					if (re > 4) re = 1;
					switch (re){
						case 1:	gfx_color(255,0,0);
							gfx_fill_rectangle(160, 240, 70, 70);
							break;
						case 2: gfx_color(0,255,0);
							gfx_fill_rectangle(160, 240, 70, 70);
							break;
						case 3: gfx_color(0,0,255);
							gfx_fill_rectangle(160, 240, 70, 70);
							break;
						case 4: gfx_color(255,230,0);
							gfx_fill_rectangle(160, 240, 70, 70);
					}
				}			// vvv gr box
				else if ((x >= 250 && x <=320) && (y >= 240 && y <= 310)) {
					gr++;
					if (gr > 4) gr = 1;
					switch (gr){
						case 1:	gfx_color(255,0,0);
							gfx_fill_rectangle(250, 240, 70, 70);
							break;
						case 2: gfx_color(0,255,0);
							gfx_fill_rectangle(250, 240, 70, 70);
							break;
						case 3: gfx_color(0,0,255);
							gfx_fill_rectangle(250, 240, 70, 70);
							break;
						case 4: gfx_color(255,230,0);
							gfx_fill_rectangle(250, 240, 70, 70);
					}
				}			 // vvv bl box
				else if ((x >= 160 && x <=230) && (y >= 330 && y <= 400)) {
					bl++;
					if (bl > 4) bl = 1;
					switch (bl){
						case 1:	gfx_color(255,0,0);
							gfx_fill_rectangle(160, 330, 70, 70);
							break;
						case 2: gfx_color(0,255,0);
							gfx_fill_rectangle(160, 330, 70, 70);
							break;
						case 3: gfx_color(0,0,255);
							gfx_fill_rectangle(160, 330, 70, 70);
							break;
						case 4: gfx_color(255,230,0);
							gfx_fill_rectangle(160, 330, 70, 70);
					}
				}			// vvv ye box
				else if ((x >= 250 && x <=320) && (y >= 330 && y <= 400)) {
					ye++;
					if (ye > 4) ye = 1;
					switch (ye){
						case 1:	gfx_color(255,0,0);
							gfx_fill_rectangle(250, 330, 70, 70);
							break;
						case 2: gfx_color(0,255,0);
							gfx_fill_rectangle(250, 330, 70, 70);
							break;
						case 3: gfx_color(0,0,255);
							gfx_fill_rectangle(250, 330, 70, 70);
							break;
						case 4: gfx_color(255,230,0);
							gfx_fill_rectangle(250, 330, 70, 70);
					}
				}
			}
			if (re == 1 && gr == 2 && bl == 3 && ye == 4)  r->key = 1;
		}
		else if (r->key == 1) {
			gfx_color(28,28,28);
			gfx_fill_rectangle(140, 220, 200, 200);//open box
			gfx_color(0,0,0);
			gfx_rectangle(140, 220, 200, 200);//box outline
			key(200, 360);
			gfx_cleararea(0, 0, 639, 100);
			gfx_color(255,255,255);
			gfx_text(40, 40, "You found the key!");
			gfx_text(40, 55, "Click on the key to add it to your toolbox.");
			c = gfx_wait();
			if (c == 1){
				x = gfx_xpos();
				y = gfx_ypos();
				if ((x >= 140 && x <= 340) && (y >= 220 && y <= 370)) {
					gfx_color(28,28,28);	// vvv key out of box
					gfx_fill_rectangle(140, 220, 200, 200);
					key(680, 520);//put key in toolbox
					r->key = 2;
				}
			}
		}
		else if (r->key == 2 || r->key == 3 || r->key == 4) { //rug revisited
			gfx_color(28,28,28);
			gfx_fill_rectangle(140, 220, 200, 200);//open box
			gfx_color(0,0,0);
			gfx_rectangle(140, 220, 200, 200);//box outline
		}
		c = gfx_wait();
		if (c == 1){
			x = gfx_xpos();
			y = gfx_ypos();		// vvv click back arrow
			if ((x >= 80 && x <= 120) && (y >= 480 && y <= 520)) {
				gfx_cleararea(0, 0, 639, 100);
				basicRoom(r);
				exit = 1;
			}
		}
	}
	return;
}

void door (struct Room* r) {
	gfx_cleararea(40, 120, 560, 440);
	gfx_color(248, 255, 174);
	gfx_fill_rectangle(80, 160, 480, 360);
	XPoint door[] = {{200,480},{200,200},{360,200},{360,480},{200,480}};
	XPoint opendoor[] = {{200,200},{240,140},{240,540},{200,480},{200,200}};
	gfx_color(112,123,141);
	gfx_fill_polygon(door, 5);
	gfx_color(0,0,0);
	gfx_polygon(door, 5);
	backarrow();
	char c;
	int exit = 0, x, y;
	while (exit == 0){
		if (r->key == 0 || r->key == 1){
			gfx_cleararea(0, 0, 439, 100);
			gfx_color(255,255,255);
			gfx_text(40, 40, "You must find a key to unlock the door.");
			keyhole(340, 340);
		}
		else if (r->key == 2) {
			c = gfx_wait();
			gfx_cleararea(0, 0, 439, 100);
			keyhole(340, 340);
			if (c == 1){
				x = gfx_xpos();
				y = gfx_ypos();
				if ((x >= 650 && x <= 790) && (y >= 470 && y <= 590)){
					gfx_color(0,242,16);
					gfx_rectangle(650, 470, 140, 120); //select key
					r->key = 3;
				}
			}
		}
		else if (r->key == 3){
			c = gfx_wait();
			if (c == 1){
				x = gfx_xpos();
				y = gfx_ypos();
				if ((x >= 320 && x <= 360) && (y >= 320 && y <= 360)){
					gfx_cleararea(650, 470, 141, 121); //out of toolbox
					r->key = 4;
					openDoor();
				}
			}
		}
		c = gfx_wait();
		if (c == 1){
			x = gfx_xpos();
			y = gfx_ypos();		// vvv click back arrow
			if ((x >= 80 && x <= 120) && (y >= 480 && y <= 520)) {
				gfx_cleararea(0, 0, 639, 100);
				basicRoom(r);
				exit = 1;
			}
		}
	}
	return;
}

void colorcard(int x, int y) {
	gfx_color(255,255,255);
	gfx_fill_rectangle(x-60, y-80, 120, 160);
	gfx_color(255,0,0);
	gfx_fill_rectangle(x-40, y-60, 30, 50);
	gfx_color(0,255,0);
	gfx_fill_rectangle(x+10, y-60, 30, 50);
	gfx_color(0,0,255);
	gfx_fill_rectangle(x-40, y+10, 30, 50);
	gfx_color(255,230,0);
	gfx_fill_rectangle(x+10, y+10, 30, 50);
}

void key(int x, int y){
	gfx_color(189,189,0);
	XPoint key[] = {{x,y-5},{x+90,y-5},{x+90,y+15},{x+80,y+15},{x+80,y+5},{x+70,y+5},
			{x+70,y+15},{x+60,y+15},{x+60,y+5},{x,y+5}};
	gfx_fill_polygon(key, 10);
	gfx_fill_circle(x-10, y, 10);
	gfx_fill_circle(x, y-10, 10);
	gfx_fill_circle(x, y+10, 10);

}

void openDoor(){
	gfx_cleararea(40, 120, 560, 440);
	gfx_color(248, 255, 174); //wall
	gfx_fill_rectangle(80, 160, 480, 360);
	XPoint door[] = {{200,480},{200,200},{360,200},{360,480},{200,480}};
	XPoint opendoor[] = {{200,200},{240,140},{240,540},{200,480},{200,200}};
	backarrow();
	gfx_color(28,28,28);
	gfx_fill_polygon(door, 5); //black area behind door
	gfx_color(112,123,141);
	gfx_fill_polygon(opendoor, 5); //open door
	gfx_color(0,0,0);
	gfx_polygon(opendoor, 5); //door outline
	gfx_color(255, 255, 255);
	gfx_changefont("9x15bold");
	gfx_text(40, 40, "You did it!");
	gfx_text(40, 60, "Congratulations!");
	char c;
	int exit = 0, x, y;
	while (exit == 0){
		c = gfx_wait();
		if (c == 1){
			x = gfx_xpos();
			y = gfx_ypos();		// vvv click back arrow
			if ((x >= 80 && x <= 120) && (y >= 480 && y <= 520)) { 
				gfx_cleararea(0, 0, 639, 100);
				exit = 1;
			}
		}
	}
}
