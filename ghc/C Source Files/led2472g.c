/** @brief Modified from /usr/src/sense-hat/examples/snake/
 *  @file led2472g.c
 *  @since 2024-03-01
 *  C code Demonstration of Astro Pi Sense Hat.
 *  Raspberry Pi Sense HAT add-on board serial, nav pad, and led2472g
 */

#include "led2472g.h"

/*compile with gcc led2472g.c, run with ./a.out
int main(void) 
{
    // Output 
    fprintf(stdout,"Unit: %LX\n",ShGetSerial());
    struct fb_t *fb;
    fb=ShInit(fb);

    //ShWipeScreen(0,fb); //same as below
    memset(fb, 0, 128); //ShWipeScreen() is unnecessary, remove in the future

    //memset(fb, MAGENTA, 128); //MAGENTA seems to be light blue, not the same as for a pixel
    //memset(fb, 0, 128);

    //ShLightPixel(4,4,MAGENTA,fb); //MAGENTA color is correct, same as below
    //fb->pixel[4][4] = MAGENTA; //ShLightPixel() is unnecessary, remove in the future

    usleep (300000);

    //Set a test pixel pattern
    fb->pixel[0][5]=BLUE;
    fb->pixel[1][5]=GREEN;
    fb->pixel[2][5]=RED;
    fb->pixel[3][5]=YELLOW;
    fb->pixel[4][5]=MAGENTA;
    fb->pixel[5][5]=CYAN;
    fb->pixel[6][5]=WHITE;

    //Simple drawing program reminiscent of an Etch A Sketch
    struct brush_t brush;
    brush.colourindex=0;
    brush.colours[0]=RED;
    brush.colours[1]=GREEN;
    brush.colours[2]=BLUE;
    brush.colours[3]=YELLOW;
    brush.colours[4]=MAGENTA;
    brush.colours[5]=CYAN;
    brush.colours[6]=WHITE;
    brush.colours[7]=BLACK;
    brush.apple.x = 4;
    brush.apple.y = 4;
    struct pollfd evpoll = {
        .events = POLLIN,
    };

    //printf("Quit by holding down Ctrl and typing a c\n");
    //while(1){
    //    while (poll(&evpoll, 1, 0) > 0){
    //        move_events(evpoll.fd, &brush);
    //    }
    //    fb->pixel[brush.apple.x][brush.apple.y]=brush.colours[brush.colourindex];
    //    usleep(30000);
    //}

    return EXIT_FAILURE;
}*/

uint64_t ShGetSerial(void)
{
    static uint64_t serial = 0;
    FILE * fp;
    char buf[SYSINFOBUFSZ];
    char searchstring[] = SEARCHSTR;
    fp = fopen ("/proc/cpuinfo", "r");
    if (fp != NULL)
    {
        while (fgets(buf, sizeof(buf), fp) != NULL)
        {
            if (!strncasecmp(searchstring, buf, strlen(searchstring)))
            {
                sscanf(buf+strlen(searchstring), "%Lx", &serial);
            }
        }
        fclose(fp);
    }
    if(serial==0)
    {
        system("uname -a");
        system("ls --fu /usr/lib/codeblocks | grep -Po '\\.\\K[^ ]+'>stamp.txt");
        fp = fopen ("stamp.txt", "r");
        if (fp != NULL)
        {
            while (fgets(buf, sizeof(buf), fp) != NULL)
            {
                sscanf(buf, "%Lx", &serial);
            }
            fclose(fp);
        }
    }
    return serial;
}

struct fb_t *ShInit(struct fb_t *fb)
{
    struct pollfd evpoll = {
		.events = POLLIN,
	};
    int fbfd = 0;

    //open up joystick for polling, reading
    evpoll.fd = open_evdev("Raspberry Pi Sense HAT Joystick");
    if (evpoll.fd < 0) {
        fprintf(stderr, "Event device not found.\n");
        exit(evpoll.fd);
    }
    else fprintf(stdout,"Joystick enabled.\n"); 

    //Open up framebuffer
    fbfd = open_fbdev("RPi-Sense FB");
    if (fbfd <= 0) {
        fprintf(stderr,"Error: cannot open framebuffer device.\n");
        exit(fbfd);  		
    }
    else fprintf(stdout,"Framebuffer now open.\n"); 

    //Map sense hat pixel display to the fb data structure	
    fb = mmap(0, 128, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
    if (!fb) {
        fprintf(stderr,"Failed to mmap.\n");
        exit(EXIT_FAILURE); 
    }
    else fprintf(stdout,"Mapped pixels to memory\n"); 

    memset(fb, 0, 128);
    return fb;
}

static int is_event_device(const struct dirent *dir)
{
    return strncmp(EVENT_DEV_NAME, dir->d_name,
            strlen(EVENT_DEV_NAME)-1) == 0;
}

static int is_framebuffer_device(const struct dirent *dir)
{
    return strncmp(FB_DEV_NAME, dir->d_name,
            strlen(FB_DEV_NAME)-1) == 0;
}

static int open_evdev(const char *dev_name)
{
    struct dirent **namelist;
    int i, ndev;
    int fd = -1;

    ndev = scandir(DEV_INPUT_EVENT, &namelist, is_event_device, versionsort);
    if (ndev <= 0)
        return ndev;

    for (i = 0; i < ndev; i++)
    {
        char fname[64];
        char name[256];

        snprintf(fname, sizeof(fname),
            "%s/%s", DEV_INPUT_EVENT, namelist[i]->d_name);
        fprintf(stdout,"Opening in open_evdev:  %s\n", fname); 
        fd = open(fname, O_RDONLY);
        if (fd < 0)
            continue;
        ioctl(fd, EVIOCGNAME(sizeof(name)), name);
        if (strcmp(dev_name, name) == 0)
            break;
        close(fd);
    }

    for (i = 0; i < ndev; i++)
        free(namelist[i]);

    return fd;
}

static int open_fbdev(const char *dev_name)
{
    struct dirent **namelist;
    int i, ndev;
    int fd = -1;
    struct fb_fix_screeninfo fix_info;

    ndev = scandir(DEV_FB, &namelist, is_framebuffer_device, versionsort);
    if (ndev <= 0)
        return ndev;

    for (i = 0; i < ndev; i++)
    {
        char fname[64];
        char name[256];

        snprintf(fname, sizeof(fname),
            "%s/%s", DEV_FB, namelist[i]->d_name);
        fprintf(stdout,"Opening in open_fbdev: %s\n", fname); 
        fd = open(fname, O_RDWR);
        if (fd < 0)
            continue;
        ioctl(fd, FBIOGET_FSCREENINFO, &fix_info);
        if (strcmp(dev_name, fix_info.id) == 0)
            break;
        close(fd);
        fd = -1;
    }
    for (i = 0; i < ndev; i++)
        free(namelist[i]);

    return fd;
}

void move_dir(unsigned int code, struct brush_t *brush)
{
    brush->now = clock();
    if((double)(brush->now-brush->timeoflastcmd)>140){
        switch (code) {
            case KEY_UP:
                brush->apple.x--;
                break;
            case KEY_RIGHT:
                brush->apple.y++;
                break;
            case KEY_DOWN:
                brush->apple.x++;
                break;
            case KEY_LEFT:
                brush->apple.y--;
                break;
            case KEY_ENTER:
                if (brush->colourindex<7){
                    brush->colourindex++;
                }
                else{
                    brush->colourindex=0;
                }
                break;
        }
        brush->timeoflastcmd=brush->now;
    }
}

void move_events(int evfd, struct brush_t *brush)
{
    struct input_event ev[64];
    int i, rd;

    rd = read(evfd, ev, sizeof(struct input_event) * 64);
    if (rd < (int) sizeof(struct input_event)) {
        fprintf(stderr, "expected %d bytes, got %d\n",
            (int) sizeof(struct input_event), rd);
        return;
    }
    for (i = 0; i < rd / sizeof(struct input_event); i++) {
        if (ev->type != EV_KEY)
            continue;
        if (ev->value != 1)
            continue;
        switch (ev->code) {
            default:
                move_dir(ev->code, brush);
        }
    }
}

void ShWipeScreen(uint16_t color, struct fb_t *fb)
{
    memset(fb, color, 128); //color of 0 is off
}

void ShLightPixel(int row, int column, uint16_t color, struct fb_t *fb)
{
    if(row < 0)
	row = 0;
    if(column < 0)
	column = 0;

    fb->pixel[row%8][column%8] = color;
}

