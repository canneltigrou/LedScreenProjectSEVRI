// This code is public domain
// (but note, that the led-matrix library this depends on is GPL v2)

#include "../submodule_rpi-rgb-led-matrix/include/led-matrix.h"
#include "../submodule_rpi-rgb-led-matrix/include/threaded-canvas-manipulator.h"
#include "../submodule_rpi-rgb-led-matrix/include/pixel-mapper.h"
#include "../submodule_rpi-rgb-led-matrix/include/graphics.h"
#include "ABE_IoPi.h"


#include <unistd.h>
#include <iostream>
#include <fstream>
#include <assert.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <algorithm>
#include <list>
#include <cstdint>
#include <exception>

// To check the date and write a message in function
#include <ctime>
#include <iostream>


using namespace rgb_matrix;
using namespace std;

// value corrseponds to the value of the frequency to display
uint16_t value = 42;
// history contains the 8 last states (corresponding to the displayed smileys)
std::list<int> history;

volatile bool interrupt_received = false;
static void InterruptHandler(int signo) {
    interrupt_received = true;
}

// 10 * 7 booleans to represent each digit, used to display the frequency
int digit[10][10][7] =
{
    { //0
        { 0, 0, 1, 1, 1, 0, 0 },
        { 0, 1, 1, 0, 1, 1, 0 },
        { 1, 1, 0, 0, 0, 1, 1 },
        { 1, 1, 0, 0, 0, 1, 1 },
        { 1, 1, 0, 0, 0, 1, 1 },
        { 1, 1, 0, 0, 0, 1, 1 },
        { 1, 1, 0, 0, 0, 1, 1 },
        { 1, 1, 0, 0, 0, 1, 1 },
        { 0, 1, 1, 0, 1, 1, 0 },
        { 0, 0, 1, 1, 1, 0, 0 }
    },
    { //1
        { 0, 0, 0, 1, 1, 0, 0 },
        { 0, 0, 1, 1, 1, 0, 0 },
        { 0, 1, 1, 1, 1, 0, 0 },
        { 0, 0, 0, 1, 1, 0, 0 },
        { 0, 0, 0, 1, 1, 0, 0 },
        { 0, 0, 0, 1, 1, 0, 0 },
        { 0, 0, 0, 1, 1, 0, 0 },
        { 0, 0, 0, 1, 1, 0, 0 },
        { 0, 0, 0, 1, 1, 0, 0 },
        { 0, 1, 1, 1, 1, 1, 1 }
    },
    { // 2
        { 0, 1, 1, 1, 1, 1, 0 },
        { 1, 1, 0, 0, 0, 1, 1 },
        { 1, 1, 0, 0, 0, 1, 1 },
        { 0, 0, 0, 0, 0, 1, 1 },
        { 0, 0, 0, 0, 1, 1, 0 },
        { 0, 0, 0, 1, 1, 0, 0 },
        { 0, 0, 1, 1, 0, 0, 0 },
        { 0, 1, 1, 0, 0, 0, 0 },
        { 1, 1, 0, 0, 0, 0, 0 },
        { 1, 1, 1, 1, 1, 1, 1 }
    },
    {  //3
        { 1, 1, 1, 1, 1, 1, 1 },
        { 0, 0, 0, 0, 0, 1, 1 },
        { 0, 0, 0, 0, 1, 1, 0 },
        { 0, 0, 0, 1, 1, 0, 0 },
        { 0, 0, 1, 1, 1, 1, 0 },
        { 0, 0, 0, 0, 0, 1, 1 },
        { 0, 0, 0, 0, 0, 1, 1 },
        { 0, 0, 0, 0, 0, 1, 1 },
        { 1, 1, 0, 0, 0, 1, 1 },
        { 0, 1, 1, 1, 1, 1, 0 }
    },
    {  //4
        { 0, 0, 0, 0, 1, 1, 0 },
        { 0, 0, 0, 1, 1, 1, 0 },
        { 0, 0, 1, 1, 1, 1, 0 },
        { 0, 1, 1, 0, 1, 1, 0 },
        { 1, 1, 0, 0, 1, 1, 0 },
        { 1, 1, 0, 0, 1, 1, 0 },
        { 1, 1, 1, 1, 1, 1, 1 },
        { 0, 0, 0, 0, 1, 1, 0 },
        { 0, 0, 0, 0, 1, 1, 0 },
        { 0, 0, 0, 0, 1, 1, 0 }
    },
    {  //5
        { 1, 1, 1, 1, 1, 1, 1 },
        { 1, 1, 0, 0, 0, 0, 0 },
        { 1, 1, 0, 0, 0, 0, 0 },
        { 1, 1, 1, 1, 1, 1, 0 },
        { 1, 1, 1, 0, 0, 1, 1 },
        { 0, 0, 0, 0, 0, 1, 1 },
        { 0, 0, 0, 0, 0, 1, 1 },
        { 0, 0, 0, 0, 0, 1, 1 },
        { 1, 1, 0, 0, 0, 1, 1 },
        { 0, 1, 1, 1, 1, 1, 0 }
    },
    {  //6
        { 0, 0, 1, 1, 1, 1, 0 },
        { 0, 1, 1, 0, 0, 0, 0 },
        { 1, 1, 0, 0, 0, 0, 0 },
        { 1, 1, 0, 0, 0, 0, 0 },
        { 1, 1, 1, 1, 1, 1, 0 },
        { 1, 1, 1, 0, 0, 1, 1 },
        { 1, 1, 0, 0, 0, 1, 1 },
        { 1, 1, 0, 0, 0, 1, 1 },
        { 1, 1, 1, 0, 0, 1, 1 },
        { 0, 1, 1, 1, 1, 1, 0 }
    },
    {  //7
        { 1, 1, 1, 1, 1, 1, 1 },
        { 0, 0, 0, 0, 0, 1, 1 },
        { 0, 0, 0, 0, 0, 1, 1 },
        { 0, 0, 0, 0, 1, 1, 0 },
        { 0, 0, 0, 1, 1, 0, 0 },
        { 0, 0, 0, 1, 1, 0, 0 },
        { 0, 0, 1, 1, 0, 0, 0 },
        { 0, 0, 1, 1, 0, 0, 0 },
        { 0, 0, 1, 1, 0, 0, 0 },
        { 0, 0, 1, 1, 0, 0, 0 }
    },
    {  //8
        { 0, 1, 1, 1, 1, 1, 0 },
        { 1, 1, 0, 0, 0, 1, 1 },
        { 1, 1, 0, 0, 0, 1, 1 },
        { 1, 1, 0, 0, 0, 1, 1 },
        { 0, 1, 1, 1, 1, 1, 0 },
        { 1, 1, 0, 0, 0, 1, 1 },
        { 1, 1, 0, 0, 0, 1, 1 },
        { 1, 1, 0, 0, 0, 1, 1 },
        { 1, 1, 0, 0, 0, 1, 1 },
        { 0, 1, 1, 1, 1, 1, 0 }
    },
    {  //9
        { 0, 1, 1, 1, 1, 1, 0 },
        { 1, 1, 0, 0, 1, 1, 1 },
        { 1, 1, 0, 0, 0, 1, 1 },
        { 1, 1, 0, 0, 0, 1, 1 },
        { 1, 1, 0, 0, 1, 1, 1 },
        { 0, 1, 1, 1, 1, 1, 1 },
        { 0, 0, 0, 0, 0, 1, 1 },
        { 0, 0, 0, 0, 0, 1, 1 },
        { 0, 0, 0, 0, 1, 1, 0 },
        { 0, 1, 1, 1, 1, 0, 0 }
    }
};

struct Area
{
    int index_r;
    int index_c;
};

Area areas[11]; // The first 3 areas are for the digits to display the frequency.
                // The area[3:10] correspond to the history stack with the smileys

struct Pixel {
    Pixel() : red(0), green(0), blue(0) {}
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};


struct Image {
    Image() : width(-1), height(-1), image(NULL) {}
    Image(int w, int h, Pixel *im) { width = w; height = h; image = im; }
    ~Image() { Delete(); }
    void Delete() { delete[] image; Reset(); }
    void Reset() { image = NULL; width = -1; height = -1; }
    inline bool IsValid() { return image && height > 0 && width > 0; }
    const Pixel &getPixel(int x, int y) {
        static Pixel black;
        if (x < 0 || x >= width || y < 0 || y >= height) return black;
        return image[x + width * y];
    }

    int width;
    int height;
    Pixel *image;
};

// Read line, skip comments.
char *ReadLine(FILE *f, char *buffer, size_t len) {
    char *result;
    do {
        result = fgets(buffer, len, f);
    } while (result != NULL && result[0] == '#');
    return result;
}


Image readPPM(const char *filename) {
    FILE *f = fopen(filename, "r");
    // check if file exists
    if (f == NULL && access(filename, F_OK) == -1) {
        fprintf(stderr, "File \"%s\" doesn't exist\n", filename);
        exit(1);
    }
    if (f == NULL) exit(1);
    char header_buf[256];
    const char *line = ReadLine(f, header_buf, sizeof(header_buf));
#define EXIT_WITH_MSG(m) { fprintf(stderr, "%s: %s |%s", filename, m, line); \
      fclose(f); exit(2); }
    if (sscanf(line, "P6 ") == EOF)
        EXIT_WITH_MSG("Can only handle P6 as PPM type.");
    line = ReadLine(f, header_buf, sizeof(header_buf));
    int new_width, new_height;
    if (!line || sscanf(line, "%d %d ", &new_width, &new_height) != 2)
        EXIT_WITH_MSG("Width/height expected");
    int value;
    line = ReadLine(f, header_buf, sizeof(header_buf));
    if (!line || sscanf(line, "%d ", &value) != 1 || value != 255)
        EXIT_WITH_MSG("Only 255 for maxval allowed.");
    const size_t pixel_count = new_width * new_height;
    Pixel *new_image = new Pixel[pixel_count];
    assert(sizeof(Pixel) == 3);   // we make that assumption.
    if (fread(new_image, sizeof(Pixel), pixel_count, f) != pixel_count) {
        line = "";
        EXIT_WITH_MSG("Not enough pixels read.");
    }
#undef EXIT_WITH_MSG
    fclose(f);
    fprintf(stderr, "Read image '%s' with %dx%d\n", filename,
        new_width, new_height);
    Image res(new_width, new_height, new_image);
    return res;
}




// print the corresponding smiley (corresponding to im). The image is already downloaded from a file. (image_bad, image_middle and image_happy smiley images)
void printSmileyAreas(FrameCanvas *canvas, Area(&areas)[11], Image &im, int state)
{
    for (int row = 0; row < im.height; ++row)
    {
        for (int col = 0; col < im.width; ++col)
        {
            const Pixel &p = im.getPixel(col, row);
            canvas->SetPixel(areas[state].index_c + col, areas[state].index_r + row, p.red, p.green, p.blue);
        }
    }
}


// print the frequency so the 3 digits on the Areas[0:2]
void printDigitAreas(FrameCanvas * canvas, Area(&areas)[11], int number)
{
    int hundreds = (number / 100) % 10;
    int tens = (number / 10) % 10;
    int units = number % 10;
    for (int col = 0; col < 7; ++col)
    {
        for (int row = 0; row < 10; ++row)
        {
            if (digit[hundreds][row][col])
                canvas->SetPixel(col + areas[0].index_c, row + areas[0].index_r, 255, 255, 255);
            if (digit[tens][row][col])
                canvas->SetPixel(col + areas[1].index_c, row + areas[1].index_r, 255, 255, 255);
            if (digit[units][row][col])
                canvas->SetPixel(col + areas[2].index_c, row + areas[2].index_r, 255, 255, 255);
        }
    }
}

// used to convert the bit array read on the file to a number to display (for the frequency).
uint16_t bitArrayToInt32(bool arr[], int count)
{
    uint16_t ret = 0;
    int tmp;
    for (int i = 0; i < count; i++) {
        tmp = arr[i];
        ret |= tmp << (count - i - 1);
    }
    return ret;
}

ABElectronics_CPP_Libraries::IoPi initializeDigitalCard(void)
{
    try
    {
        ABElectronics_CPP_Libraries::IoPi bus1(0x20);
        printf("TEST\n");
        bus1.set_port_direction(0, 0xFF); // set bank 0 to be inputs
        bus1.set_port_direction(1, 0xFF); // set bank 1 to be inputs
        bus1.set_port_pullups(0, 0xFF);   // enable internal pullups for bank 0
        bus1.invert_port(0, 0xFF);		  // invert output so bank will read as 0
        bus1.set_port_pullups(1, 0xFF);   // enable internal pullups for bank 0
        bus1.invert_port(1, 0xFF);		  // invert output so bank will read as 0
        printf("TEST_FIN\n");
        return bus1;
    }
    catch (exception &e)
    {
        cout << e.what();
    }
}

// Insert a new state (0,1,2 corresponding to smileys) into the history stack
void InsertState(int s)
{
    history.push_front(s);
    if (history.size() > 8)
        history.pop_back();
}




// To display a text, for example at the begining of the program, 
// To say "Hello", or give wishes
int DisplayText(std::string text_to_display, FrameCanvas*& canvas, RGBMatrix* matrix)
{
    int cols = 128; // TODO initialised from the main
    int chain_length = 2;
    
    std::cout << "I am in displaytext()" << std::endl;
    std::cout <<"text to display: "<< text_to_display.c_str() << std::endl;

    
    const char *bdf_font_file = "../submodule_rpi-rgb-led-matrix/fonts/10x20.bdf";
    /* x_origin is set by default just right of the screen */
    const int x_default_start = (chain_length * cols) + 5;
    int x_orig = x_default_start;
    int y_orig = 0;
    int letter_spacing = 1;
    float speed = 8.0f;
    int loops = 1; // I want to display the text 1 time.
    int brightness = 50;

    Color color(255, 255, 0);
    Color bg_color(0, 0, 0);

    
    /*
    * Load font. This needs to be a filename with a bdf bitmap font.
    */
    rgb_matrix::Font font;
    if (!font.LoadFont(bdf_font_file)) {
        fprintf(stderr, "Couldn't load font '%s'\n", bdf_font_file);
        return 1;
    }

    // Create a new canvas to be used with led_matrix_swap_on_vsync -> already done with canvas
    FrameCanvas *offscreen_canvas = matrix->CreateFrameCanvas(); // TO delete ?

    int delay_speed_usec = 1000000 / speed / font.CharacterWidth('W');

    int x = x_orig;
    int y = y_orig;
    int length = 0;


    while (!interrupt_received && loops != 0) {
        offscreen_canvas->Fill(bg_color.r, bg_color.g, bg_color.b);
        // length = holds how many pixels our text takes up
        length = rgb_matrix::DrawText(offscreen_canvas, font,
                                    x, y + font.baseline(),
                                    color, nullptr,
                                    text_to_display.c_str(), letter_spacing);
        if (speed > 0 && --x + length < 0) {
            x = x_orig;
            if (loops > 0) --loops;
        }

        // Swap the offscreen_canvas with canvas on vsync, avoids flickering
        offscreen_canvas = matrix->SwapOnVSync(offscreen_canvas);  // TO delete ?
        usleep(delay_speed_usec);
    }
    std::cout << "I leave the function" << std::endl;
    return 0;
}



std::string DecideStringToDisplay()
{
    std::string str = "BONJOUR ! Bon courage pour cette nouvelle production ! =D";
    // We will check some dates and modify the text in funtion.
    
    std::time_t now = time(0);
    std::tm *ts = localtime(&now);
    
    int day = ts->tm_mday;
    int month = ts->tm_mon + 1;
    int year = ts->tm_year + 1900;
    
    std::string date = to_string(day) + "/" + to_string(month) + "/" + to_string(year);
    std::cout << "date : " << date << std::endl;
    // if(date=="YYYY/MM/DD")
    if(month == 1 && day < 7)
    {
        str = "BONNE ANNEE " + to_string(year) + " !";
        return str;
    }
    if(month == 12 && day > 10)
    {
        str = "JOYEUSES FETES DE FIN D'ANNEE !";
        return str;
    }
    if(month == 1 && day == 12)
    {
        str = "JOYEUX ANNIVERSAIRE GEOFFREY !";
        return str;
    }
    return str;
}






// thread running in parallel, used to display on the panel
class ImageDrawingThread : public ThreadedCanvasManipulator {
public:
    // Scroll image with "scroll_jumps" pixels every "scroll_ms" milliseconds.
    // If "scroll_ms" is negative, don't do any scrolling.
    ImageDrawingThread(RGBMatrix *m)
        : ThreadedCanvasManipulator(m),
        matrix_(m) 
    {
        offscreen_ = matrix_->CreateFrameCanvas();
    }

    virtual ~ImageDrawingThread() 
    {
        Stop();
        WaitStopped();   // only now it is safe to delete our instance variables.
    }

    // It reads the stack history and display the corresponding smileys and frequency
    void Run() 
    {
        printf("RUNING\n");
        const int screen_height = offscreen_->height();
        const int screen_width = offscreen_->width();
        
        DisplayText(DecideStringToDisplay(), offscreen_, matrix_);

        while (running() && !interrupt_received) 
        {
            offscreen_->Clear();
            //offscreen_ = matrix_->CreateFrameCanvas();
            usleep(100 * 1000);
            printDigitAreas(offscreen_, areas, value);
            if (!history.empty())
            {
                int index_area;
                std::list<int>::iterator it;
                for (it = history.begin(), index_area = 3; it != history.end(); ++it, ++index_area)
                {
                    switch(*it)
                    {
                        case 0:
                            printSmileyAreas(offscreen_, areas, image_happy, index_area);
                            break;
                        case 1:
                            printSmileyAreas(offscreen_, areas, image_sceptic, index_area);
                            break;
                        case 2:
                            printSmileyAreas(offscreen_, areas, image_unhappy, index_area);
                            break;
                        default:
                            break;
                    }
                }
                usleep(100 * 1000);
            }
            offscreen_ = matrix_->SwapOnVSync(offscreen_);
        } // end of the while loop
    } // end of run()

private:

    int32_t horizontal_position_;

    RGBMatrix* matrix_;
    FrameCanvas* offscreen_;
    ABElectronics_CPP_Libraries::IoPi *bus1_;

    // Images manipulated in our thread.
    Image image_happy = readPPM("happy.ppm");
    Image image_sceptic = readPPM("sceptic.ppm");
    Image image_unhappy = readPPM("unhappy.ppm");


};


static int usage(const char *progname) {
    fprintf(stderr, "usage: %s <options> -D <demo-nr> [optional parameter]\n",
            progname);
    fprintf(stderr, "Options:\n");
    fprintf(stderr,
            "\t-D <demo-nr>              : Always needs to be set\n"
            "\t-t <seconds>              : Run for these number of seconds, then exit.\n");


    rgb_matrix::PrintMatrixFlags(stderr);

    fprintf(stderr, "Demos, choosen with -D\n");
    fprintf(stderr, "\t0  - some rotating square\n"
            "\t1  - forward scrolling an image (-m <scroll-ms>)\n"
            "\t2  - backward scrolling an image (-m <scroll-ms>)\n"
            "\t3  - test image: a square\n"
            "\t4  - Pulsing color\n"
            "\t5  - Grayscale Block\n"
            "\t6  - Abelian sandpile model (-m <time-step-ms>)\n"
            "\t7  - Conway's game of life (-m <time-step-ms>)\n"
            "\t8  - Langton's ant (-m <time-step-ms>)\n"
            "\t9  - Volume bars (-m <time-step-ms>)\n"
            "\t10 - Evolution of color (-m <time-step-ms>)\n"
            "\t11 - Brightness pulse generator\n");
    fprintf(stderr, "Example:\n\t%s -t 10 -D 1 runtext.ppm\n"
            "Scrolls the runtext for 10 seconds\n", progname);
    return 1;
}


// read into the test.bin file (filled by the program in parallel : test_input) where the current states are logged.
// then it push the new state into the history stack.
void read_states()
{
    uint16_t last_iter = 1;
    printf("I am reading the file\n");
    while (!interrupt_received) 
    {
        ifstream input("test.bin", ios::ate);
        if (!input.is_open())
        {
            printf("THE FILE DOESN'T EXIST\n");
            sleep(5);
            continue;
        }
        else
        {
            if (input.tellg() == sizeof(bool) * (4 + 10))
            {
                printf("GOOD SIZE\n");
                input.clear();
                input.seekg(0, ios::beg);
                bool res_smiley[4];
                bool res_value[10];
                char tmp;
                for (int i = 0; i < 4; ++i)
                {
                    input.get(tmp);
                    res_smiley[i] = (bool)(tmp - 48);
                    printf("%d\n", res_smiley[i]);
                }
                for (int i = 0; i < 10; ++i)
                {
                    input.get(tmp);
                    res_value[i] = (bool)(tmp - 48);
                }
                input.close();
                    value = bitArrayToInt32(res_value, 10);
                if (last_iter != res_smiley[3])
                {
                    last_iter = res_smiley[3];
                    if (res_smiley[0])
                        InsertState(0);
                    else if (res_smiley[1])
                        InsertState(1);
                    else if (res_smiley[2])
                        InsertState(2);
                }
            }
            sleep(5); // Time doesn't really matter. The syscall will be interrupted.
        }

    }
}


int main(int argc, char *argv[]) {

    using namespace ABElectronics_CPP_Libraries;
    IoPi bus1(0x20);

    int runtime_seconds = -1;
    int scroll_ms = -1;

    areas[0].index_r = 0;
    areas[0].index_c = 1;
    areas[1].index_r = 11;
    areas[1].index_c = 1;
    areas[2].index_r = 22;
    areas[2].index_c = 1;

    for (int i = 3; i < 11; ++i)
    {
        areas[i].index_r = 1;
        areas[i].index_c = 9 + (i - 3) * 31;
    }

    RGBMatrix::Options matrix_options;

    rgb_matrix::RuntimeOptions runtime_opt;

    //  . / demo --led - chain = 2 - D 1 --led - rows = 32 --led - cols = 128 --led - brightness = 50 --led - no - hardware - pulse --led - gpio - mapping = adafruit - hat --led - slowdown - gpio = 4 smile2.ppm
    matrix_options.rows = 32;
    matrix_options.chain_length = 2;
    matrix_options.parallel = 1;
    matrix_options.cols = 128;
    matrix_options.brightness = 50;
    matrix_options.hardware_mapping = "adafruit-hat";
    matrix_options.disable_hardware_pulsing = true;
    runtime_opt.gpio_slowdown = 4;

    // First things first: extract the command line flags that contain
    // relevant matrix options.
    if (!ParseOptionsFromFlags(&argc, &argv, &matrix_options, &runtime_opt)) {
        return usage(argv[0]);
    }

    RGBMatrix *matrix = CreateMatrixFromOptions(matrix_options, runtime_opt);
    if (matrix == NULL)
        return 1;

    printf("Size: %dx%d. Hardware gpio mapping: %s\n",
        matrix->width(), matrix->height(), matrix_options.hardware_mapping);

    Canvas *canvas = matrix;

    // The ThreadedCanvasManipulator objects are filling
    // the matrix continuously.
    ThreadedCanvasManipulator *image_gen = NULL;

    // Set up an interrupt handler to be able to stop animations while they go
    // on. Note, each demo tests for while (running() && !interrupt_received) {},
    // so they exit as soon as they get a signal.
    signal(SIGTERM, InterruptHandler);
    signal(SIGINT, InterruptHandler);

    ImageDrawingThread *imt = new ImageDrawingThread(matrix);		

    image_gen = imt;

    if (image_gen == NULL)
        return usage(argv[0]);

    // Image generating demo is created. Now start the thread.
    image_gen->Start();

    // Now, the image generation runs in the background. We can do arbitrary
    // things here in parallel. In this demo, we're essentially just
    // waiting for one of the conditions to exit.
    if (runtime_seconds > 0) {
        sleep(runtime_seconds);
    }
    else {
        read_states();
    }

    // Stop image generating thread. The delete triggers
    delete image_gen;
    delete canvas;

    printf("\%s. Exiting.\n",
        interrupt_received ? "Received CTRL-C" : "Timeout reached");
    return 0;
}
