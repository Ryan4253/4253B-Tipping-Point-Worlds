# 4253B-Tipping-Point-Worlds
<div align='center'>
<img src="https://user-images.githubusercontent.com/71594512/168233814-f5237318-e351-4881-9610-1f461b6c7823.png" width=75% height=75% alt="award">
</div>

## Features
1. Rather than using traditional tank or arcade control, we used something called curvature drive instead. Essentially, the left stick controls the throttle and the right stick controls the curvature (inverse of radius) the robot drives in. You basically control how large of an arc your robot drives in, and increasing speed doesn't affect the rate you're curving. (The implementation can be found in src/ryanlib/ExpandedSkidSteerModel.cpp)
2. For autonomous, we controlled our chassis using what is called an S Curve Motion Profile. By inputting the desired distance to travel, as well as the robot's max velocity, acceleration and jerk, it is able to generate a set of velocity that respects the robot's kinematic constraints and travels the desired distance when followed perfectly. Since the kinematic constraints are obeyed, minimal slipping will occur, improving the overall accuracy and precision of the chassis.
3. Our code also allows curved motions by creating trajectories. Our trajectories are generated given a bezier curve and the robot's kinematic constraints. Similar to the S Curve Motion Profile above, it also generates a set of velocity that when followed, will follow the bezier curve perfectly. Since the trajectories are generated from our computers, then pasted into this code, so you won't see any trajectory generation code here.
4. Because the S Curve Motion Profiles output target velocity and acceleration, we needed some way to feed the output to the motor. We used a mainly feedforward based controller that takes into consideration the target velocity, acceleration, and position as well as the current motor state to accurately control the motor velocities. With motion profiles and our velocity controller, we were able to get our control to be within 0.1" of accuracy and basically 0 variance.
5. Another feature of our code is that all the subsystems are asynchronous. By using threads, the control functions are all non-blocking, meaning we are able to control multiple subsystems at once, improving the efficiency of our runs. The code is also thread safe through the use of mutexes.
6. We also made an auton selector that uses an SD Card.
7. The code is unit checked using the unit framework included in okapi to make sure that dimensional analysis matches up.
8. Very clean code, with everything documented and written in classes. Most of our motion control codes are contained within the ryanlib folder (which is built upon okapilib). By simply applying the library and giving new system constraints, we are able to use the same code again on different robots. For example, below are the code we use to create our S Curve Motion Profile controllers:

```cpp
MotorGroup leftDrive({-3, -2});
MotorGroup rightDrive({17, 20});
ProfileConstraint moveLimit({3_ftps, 7_ftps2, 7_ftps2, 30_ftps3});

auto chassis = ChassisControllerBuilder()
    .withMotors(leftDrive, rightDrive)
    .withDimensions({AbstractMotor::gearset::green, 5.0/7.0}, {{3.25_in, 1.294_ft}, imev5BlueTPR})
    .build();

auto profiler = AsyncMotionProfilerBuilder()
    .withOutput(chassis)
    .withProfiler(std::make_unique<SCurveMotionProfile>(moveLimit))
    .build();
```

## Some images of our robot (videos can be found in the description tab on the right) 

<img src="https://user-images.githubusercontent.com/71594512/168233164-d95b95dc-4042-46c4-89d5-a4199ab93cf5.jpg" width=62% height=62%> <img src="https://user-images.githubusercontent.com/71594512/168233193-c16d4737-57b9-43e3-a464-d65ec3495920.jpg" width=35% height=35%>

## Future Plans
Our team is made up of 4 juniors and 1 sophomore. However, we are not sure if we will continue competing next year. I will most likely just move onto mentoring younger teams and developing my code instead. Most of the reusable code written this year will be incorporated into [lib4253](https://github.com/Ryan4253/lib4253/tree/develop
), another programming library I've been developing  since around February last year. It includes powerful features such as pure pursuit, RAMSETE (and hopefully custom trajectory generation too), and I plan on finishing the code around summer / early Spin Up and releasing it to the public.

## Past season code if anyone is interested in seeing how we improved
Change Up Early Season: https://github.com/Yessir120/Change-Up <br>
Change Up Mid Season: https://github.com/Yessir120/TAStonishing_Change-Upmk2 <br>
Tipping Point Pre-Worlds: https://github.com/Ryan4253/4253B-Tipping-Point <br>
