import serial
import pygame
import time

# activate the pygame library .
scale = 3
pygame.init()
clock = pygame.time.Clock()
X = 437*scale
Y = 250*scale

# create the display surface object
# of specific dimension..e(X, Y).
scrn = pygame.display.set_mode((X, Y+(50*scale)))
# set the pygame window name
pygame.display.set_caption('golazo')

# load and scale the images for ball and goal
gol = pygame.image.load('goal_graphic.jpg')
gol = pygame.transform.scale(gol, (X, Y))
ball = pygame.image.load('ball.png')
ball = pygame.transform.scale(ball, (50*scale,50*scale))
# Using blit to display the goal
scrn.blit(gol, (0, 0))

# initial ball position and speed before game loop begins
data = ''
xpos = 0
ypos = Y
speed = 0

# set up font
font = pygame.font.Font('freesansbold.ttf', 30)

# set up serial data
dweenSerial = serial.Serial('/dev/cu.usbmodem144101', 9600) # replace with port and baudrate being used

# paint screen one time
pygame.display.flip()
status = True
while (status):
    for i in pygame.event.get():
        if i.type == pygame.QUIT:
            status = False
        # checks for right click to reset the data
        elif i.type == pygame.MOUSEBUTTONDOWN and i.button == 3:
            data = ''

    # refresh the screen with a white background
    scrn.fill((255,255,255))
    # display the goal again
    scrn.blit(gol, (0, 0))

    # read the data from the arduino serial line until we get a string
    while (data == ''):
        data = dweenSerial.readline()
        print(type(data))
    split_data = data.split(',')
    xpos = float(split_data[0])
    ypos = float(split_data[1])


    # display the ball at its current position
    x1 = xpos
    y1 = ypos
    xpos = (xpos/72)*X
    ypos = Y - (ypos/48)*Y
    scrn.blit(ball, (xpos, ypos))
    
    # display the text
    text = font.render('Your Shot\'s location: ({},{}) '.format(x1, y1), True, (0,0,0))
    textRect = text.get_rect()
    textRect.center = (X//2, Y+25*scale)
    scrn.blit(text, textRect)
    # update the screen, lock updates at 60 times/second
    pygame.display.update()
    clock.tick(60)
    data = ''
    text = font.render("", True, (0,0,0))
        
# deactivates the pygame library
pygame.quit()
