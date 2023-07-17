# AngleEstimator
AngleEstimator

Estimates the angle of straight-lined objects such as pens, rectangular boxes, etc relative to the horizontal axis of the camera

HSV Color masking concept inspired by: @murtazasworkshop (on Youtube).

Rest of the code is implemented by me

1. press C to exit
2. this version has only 2 colors as default (orange and mint), to add more colors, simply add the hsv range values to the vector
3. This is a rough estimation, not an accurate one
4. Likewise, objects with straight line edges will be generate more accurate results
5. this program only detects 0~90 degrees, once the object tilts over 90 degrees, it will measure the opposite side
(i.e., if object is at roughly 110 degrees, it will measure 70 degrees; However, no direction is given)
6. No GUI, sorry :)
