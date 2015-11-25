%Tone Map

pkg load image

original_image = imread("../src/images/img001.jpg");
image_size = size(original_image);
height = image_size(1,1);
width = image_size(1,2);
if(length(image_size) == 3)
    R = double(original_image(:,:,1));
    G = double(original_image(:,:,2));
    B = double(original_image(:,:,3));
    L = 0.27*R + 0.67*G + 0.06*B;
else
    disp('Not a color image');
    return;
end

invL = 1./(L);
R1 = R .* invL;
G1 = G .* invL;
B1 = B .* invL;

a = 10;  %gain of illumination correction
b = 10;  %coefficient of illumination correction
Lmax = 255;

%To general illumination
L1 = log(L * b * (a-1) + 1) / log(a);

%To low lightness processing, we use the following equation
%L1 = log(L/Lmax * (a-1) + 1) / log(a) * 255;

%To high lightness processing, we use the following equation
%L1 = (a.^(L/Lmax) - 1) / (a-1) * Lmax;

R2 = R1 .* L1;
G2 = G1 .* L1;
B2 = B1 .* L1;

enhanced_image = cat(3,R2,G2,B2) / 255;

figure('Name','Original Image','Numbertitle','off');
imshow(original_image);

figure('Name','Tone Map','Numbertitle','off');
imshow(enhanced_image);

waitforbuttonpress;
