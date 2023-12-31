// Helpful debug directives. They will print things to the serial monitor to aid in debugging wireless
#define DEBUG
#define SEND_DEBUG
#define RECV_DEBUG
// #define UDP_RECV_DEBUG
#define UDP_RECV_DEBUG_DEEP

#include "rcc_stdlib.h"
#include <rcc_wireless_msg_interface.h>

using namespace std;
uint32_t Ts = 1000;
float power;
float left_power;
float right_power;
float theta = 0;
float wireless = 1;

typedef enum{
    INITIAL,
    WIRELESS,
    SEARCH,
    TRACK
} state_t;

/// @brief Demux for the incoming packets
/// @param p : A message in packet form
void packet_receiver(Packet p)
{
    switch (p.id())
    { // Switch on the id of the packet
    case 0:
        break;

    case Test_Outbound::id:
    {
#ifdef DEBUG
        printf("[DEBUG]: TEST OUTBOUND RECEIVED!\n");
#endif
        break;
    }

    case Twist::id:
    {
        Twist twist(p);
#ifdef RECV_DEBUG
        printf("[RECV_DEBUG]: %s", twist.repr().c_str());
        if (twist.linear > 1)
        {
            cyw43_arch_gpio_put(0, false);
            sleep_ms(200);
            cyw43_arch_gpio_put(0, true);
        }
#endif
        break;
    }

        //     case Move::id:
        //     {
        //         Move move(p);
        // #ifdef RECV_DEBUG
        //         printf("[RECV_DEBUG]: %s", move.repr().c_str());
        //         power = move.power;

        // #endif
        //         break;
        // }
    case Move::id:
    {
        Move move(p);
#ifdef RECV_DEBUG
        printf("[RECV_DEBUG]: %s", move.repr().c_str());
        left_power = (move.left);
        right_power = (move.right);

#endif
        break;
    }
    case State::id:
    {
        State state(p);
#ifdef RECV_DEBUG
        printf("[RECV_DEBUG]: %s", state.repr().c_str());
        wireless = (state.wireless);
#endif
        break;
    }

    case Position::id:
    {
        break;
    }

    case Sensor_Data::id:
    {
        Sensor_Data data(p);
#ifdef RECV_DEBUG
        cout << "[RECV_DEBUG]: " << data.repr().c_str();
#endif
        break;
    }

    case Stop::id:
    {
    }

    default:
// nothing (yet, at least)
#ifdef RECV_DEBUG
        printf("[RECV_DEBUG]: Invalid Packet | id: %u | data: %s\n", p.id(), p.data().c_str());
#endif
        break;
    }
}

/// @brief Initialize wireless module to connect. TODO: Put other cyw43 related startup funcs here
/// @return True:success, False:failure
bool init_cyw43()
{
    // Attempt connection
    for (int i = 0; i < 10; i++)
    {
        cout << "Connection attempt #" << i << "\n";
        if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 10000))
        {
            cout << "Failed to connect to:" << WIFI_SSID << " with pass: " << WIFI_PASSWORD << "\n";
            ip_addr_t temp;
            ipaddr_aton("0.0.0.0", &temp);
            if (netif_list->ip_addr.addr == temp.addr)
            {
                cout << "Got useless ip addr (0.0.0.0), retrying\n";
            }
            else
            {
                return false;
            }
        }
        else
        {

            return true;
        }
    }
    return false;
}

typedef struct robot_conglomerate_s
{
    WirelessMsgInterface *interface;
    MPU6050 *imu;
    VL53L0X *lidar;
    Left_Odom *left;
    Right_Odom *right;

} robot_conglomerate_t;

robot_conglomerate_t *default_robot_conglomerate()
{
    robot_conglomerate_t *robot = (robot_conglomerate_t *)malloc(sizeof(robot_conglomerate_t));

    WirelessMsgInterface *interface = new WirelessMsgInterface(COMP_IP, PICO_IP, TO_COMP_PORT, TO_PICO_PORT);
    interface->setup_wireless_interface();
    MPU6050 *imu = new MPU6050();
    imu->begin(i2c1);

    robot->interface = interface;
    robot->imu = imu;
    return robot;
}

typedef struct robot_sensor_data_s
{
    uint16_t pot;
    float wz;
} robot_sensor_data_t;

bool send_test_message(repeating_timer_t *t)
{
    // Cast user data from void pointer to interface
    WirelessMsgInterface *interface = (WirelessMsgInterface *)t->user_data;
    // Create a dummy message
    Sensor_Data data;
    uint32_t time = time_us_32();
    uint32_t potval = (uint32_t)adc_read();
    data.time = (int32_t)time;
    data.theta = 1.0;
    data.wz = 2.0;
    data.potval = potval;
    data.left = 3;
    data.right = 4;
    data.dist = 5;
    // Send data over interface (pass in packet form of message)
    interface->send_msg(data.pack());
    return true;
}


int main()
{
    uint delay_length;
    repeating_timer_t send_timer;
    rcc_init_i2c();
    rcc_init_potentiometer();
    stdio_init_all();
    sleep_ms(1000);

    // Initialize the wireless hardware
    if (cyw43_arch_init())
    {
        cout << "Failed to initialise, aborting\n";
        return 1;
    }
    cout << "Initialized cyw43 arch\n";
    cyw43_arch_gpio_put(0, 1);
    cyw43_arch_enable_sta_mode();
    init_cyw43(); // Attempts connection to wireless access point

    Motor motors;
    MotorInit(&motors, RCC_ENA, RCC_ENB, 1000);
    MotorsOn(&motors);

    RGBLED leds;
    LEDinit(&leds, RCC_LED_BLUE, RCC_LED_RED, RCC_LED_GREEN, 1000);
    LEDOn(&leds);

    // Initialize lidar
    VL53L0X lidar;
    rcc_init_lidar(&lidar);

    // Initialize Servo motor
    Servo s3;
    ServoInit(&s3, 18, false, 50);
    ServoOn(&s3);

    // Instantiate wireless interface class
    WirelessMsgInterface interface(COMP_IP, PICO_IP, TO_COMP_PORT, TO_PICO_PORT);
    interface.setup_wireless_interface();

    // Turn on the LED to make sure i2c devs aren't holding the bus
    cyw43_arch_gpio_put(0, 1);

    // Have send_robot_state get called every Ts seconds
    add_repeating_timer_ms(Ts, send_test_message, &interface, &send_timer);

    // Update lwip_infra ip if router gave us a diff ip and print so we can update externally
    char *address;
    interface.lwip_infra.pico_ip = netif_list->ip_addr;
    address = ipaddr_ntoa(&interface.lwip_infra.pico_ip);
    printf("This PICO's IP address is: %s\n", address);

    //rename state_name to something more informative
    state_t state = INITIAL;

    while (true)
    {
        // Check if msg has come in, deserialize it, and take action dependent on which msg it is
        interface.get_msg_timeout(&packet_receiver, 10000);
        cout << "Move Power Val: " << power << "\n";
        cout << "Left: " << left_power << "\n";
        cout << "right: " << right_power << "\n";
        cout << "wireless: " << wireless << "\n";
        
        

        ServoPosition(&s3, 50); // face servo forward
        uint16_t dist = getFastReading(&lidar); // get lidar reading
        switch(state){
            case INITIAL:
                if (wireless == 1.0)
                 {
                    LEDPower(&leds, 100, 0, 0);
                    MotorPower(&motors, (left_power * .998), (right_power));
                }
                else if (wireless == 0.0)
                {

                    LEDPower(&leds, 0, 100, 0);
                    state = SEARCH;
                }
                break;
            case SEARCH:
                MotorPower(&motors, 70, -(.983 * 70));  // %motor speed 
                if (dist <= 500) {
                    state = TRACK;
                }
                break;
            case TRACK: 
                if (wireless == 1.0)
                 {
                    state = INITIAL;
                }
                if (dist > 400){
                    MotorPower(&motors, 70, 70);  // %motor speed 
                }
                if (dist < 300){
                    MotorPower(&motors, -70, -70);  // %motor speed 
                }
                break;
            }


        

        // Do other NON BLOCKING code here!
        cout << address << "\n";
    }

    // Exit program, deinit wireless module first
    cyw43_arch_deinit();
    return 0;
}