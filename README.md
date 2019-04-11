# NUC029xDE_LED_DIMMING
update @ 2019/04/11

Add LED dimming with PWM function (PA12)
- increase duty per 18 ms , 
- when reach 100 % duty , change to decrase duty per 18ms 
- when reach 0 % duty , change to increase duty again

Add Button switch with GPIO interrupt function (PB3)
- when press button , clear duty and output PWM duty to 0 %
- when release button , increause PWM duty from 0% per 18 ms

