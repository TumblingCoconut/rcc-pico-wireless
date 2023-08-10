from pynput import keyboard
from pynput.keyboard import Key
from comms.controller import WirelessController, WirelessInterface
from comms.messages import Twist
from comms.messages import Move

c = WirelessController(WirelessInterface)
c.start_inbound()
c.start_outbound()


def on_press(key):
    try:
        print('alphanumeric key {0} pressed'.format(
            key.char))
        if (key.char == 'q'):
            print("q was pressed")
            # make new message
            t = Twist((40, 500))
            pout = t.pack()
            c.outbound.put(pout)
        if (key.char == 'w'):
            print("w was pressed")
            # make new message
            t = Move((100.0, 100.0))
            pout = t.pack()
            c.outbound.put(pout)
        if (key.char == 's'):
            print("s was pressed")
            # make new message
            t = Move((-100.0, -100.0))
            pout = t.pack()
            c.outbound.put(pout)
        if (key.char == 'a'):
            print("a was pressed")
            # make new message
            t = Move((90.0, 100.0))
            pout = t.pack()
            c.outbound.put(pout)
        if (key.char == 'd'):
            print("d was pressed")
            # make new message
            t = Move((100.0, 90.0))
            # t = Twist((1.0, 50.0))
            pout = t.pack()
            c.outbound.put(pout)
        if (key.char == 'e'):
            print("e was pressed")
            t = Move((0.0, 0.0))
            # t = Twist((1.0, 50.0))
            pout = t.pack()  
            c.outbound.put(pout)



    except AttributeError as e:
        print('special key {0} pressed'.format(key))
        print(e)


def on_release(key):
    print('{0} released'.format(
        key))
    if key == keyboard.Key.esc:
        # Stop listener
        return False


listener = keyboard.Listener(on_press=on_press, on_release=on_release)
listener.start()

while True:
    pass
