#include <allegro.h>
#include<string.h>
#define white makecol(255, 255, 255)

enum partStatus {
     normal, 
     warning, 
     exploded
};
typedef struct  {
     char name[20];     //The printed name of the system
     int namex;         //Where the name has to be drawn
     int namey;         
     int infx;          //Where the currently selected information is drawn
     int infy;
     int bitx;          //Where the bitmap has to be drawn
     int bity;
     int temp;          //Current temp
     int cool;          //Current coolant lines
     int status;        //Is it functioning normally,
     int v;       //Volts
     int a;       //Amps
     BITMAP *sysBit[3]; //Holds all the image files, for normal functioning, dangerously hot, and exploded
} module;

module rad;             //Radiation Shields
module rcon;
module reactor;
module fcell;
module prbatt;
module sbatt;
module agrav;

BITMAP *background;
BITMAP *buffer;
BITMAP *wireCon;        //Bitmap of connected wire
BITMAP *wireDecon;      //Bitmap of disconnected wire

int radPC = 0;         //Percentage of the radshields a and b
int ticks = 0;
int wires[20] = {0};   //Array that holds the value of every wire (one meaning connected)
int wirex[20] = {234, 302};
int wirey[20] = {85, 85};
int i, j;
int keyPress, oldKeyPress;

void update();
int inv(int inv);
void draw();
void init();
void deinit();
int keyPressed(int keyValue);

void ticker() {
    ticks++;     
}
int main() {
	init();

	while (!key[KEY_ESC]) {
          while(ticks == 0) {
              rest(1);
          }
          while(ticks > 0) {
              update();
              ticks--;
          }
          draw();
	}

	deinit();
	return 0;
}
END_OF_MAIN()

void update() {
     oldKeyPress = keyPress;
     for(i = 0; i < KEY_MAX; i++) {
           if(key[i]) {
               keyPress = i;
               break;           
           }      
           else keyPress = 0;
     }
     if(rad.temp >= 100) rad.status = 1;
     if(rad.temp >= 120) rad.status = 2;
     if(key[KEY_RIGHT] && radPC < 100) radPC++;
     else if(key[KEY_LEFT] && radPC > 0) radPC--;      
     else if(keyPressed(KEY_A)) wires[0] = inv(wires[0]);    
     else if(keyPressed(KEY_B)) wires[1] = inv(wires[1]);       
}

void draw() {
     blit(background, buffer, 0, 0, 0, 0, background->w, background->h);
     for(i = 0; i < 20; i++) {
         if(wires[i]) blit(wireCon, buffer, 0, 0, wirex[i], wirey[i], wireCon->w, wireCon->h);
         else blit(wireDecon, buffer, 0, 0, wirex[i], wirey[i], wireCon->w, wireCon->h);      
     }
     blit(rad.sysBit[rad.status], buffer, 0, 0, rad.bitx, rad.bity, rad.sysBit[rad.status]->w, rad.sysBit[rad.status]->h);
     textprintf_ex(buffer, font, rad.namex, rad.namey, white, -1, "%s", rad.name);
     textprintf_ex(buffer, font, rad.infx, rad.infy, white, -1, "%10d", rad.temp);
     textprintf_ex(buffer, font, 207, 72, white, -1, "%7d %7d", radPC, radPC);
     blit(buffer, screen, 0, 0, 0, 0, buffer->w, buffer->h);
}

void init() {
	int depth, res;
	allegro_init();
	depth = desktop_color_depth();
	if (depth == 0) depth = 32;
	set_color_depth(depth);
	res = set_gfx_mode(GFX_AUTODETECT_FULLSCREEN, 1024, 768, 0, 0);
	if (res != 0) {
		allegro_message(allegro_error);
		exit(-1);
	}

	install_timer();
	install_keyboard();
	install_mouse();
	install_int_ex(ticker, BPS_TO_TIMER(20));
	strcpy(rad.name, "Rad Shields");
	rad.namex = 236;
	rad.namey = 29;
	rad.infx = 236;
	rad.infy = 52;
	rad.bitx = 229;
	rad.bity = 25;
	rad.temp = 0;
	rad.cool = 0;
	rad.v = 0;
	rad.a = 0;
	rad.status = normal;
	rad.sysBit[0] = load_bitmap("ENGfiles/rad0.bmp", NULL);
	rad.sysBit[1] = load_bitmap("ENGfiles/rad1.bmp", NULL);
	rad.sysBit[2] = load_bitmap("ENGfiles/rad2.bmp", NULL);
	wireCon = load_bitmap("ENGfiles/wireCon.bmp", NULL);
	wireDecon = load_bitmap("ENGfiles/wireDecon.bmp", NULL);
	background = load_bitmap("ENGfiles/background.bmp", NULL);
	buffer = create_bitmap(screen->w, screen->h);
}

void deinit() {
	clear_keybuf();
}

int inv(int inv) {
    if(inv == 1) return 0;
    if(inv == 0) return 1;    
}

int keyPressed(int keyValue) {
    if(keyPress == keyValue && oldKeyPress != keyValue) return 1;
    else return 0;
}
