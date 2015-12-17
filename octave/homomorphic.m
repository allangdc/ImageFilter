%Homomorphic Filter

pkg load image

original_image = imread("../src/images/img003.png");
image_size = size(original_image);
height = image_size(1,1);
width = image_size(1,2);
if(length(image_size) == 3)
    R = double(original_image(:,:,1));
    G = double(original_image(:,:,2));
    B = double(original_image(:,:,3));
    L = 0.299*R + 0.587*G + 0.114*B;
else
    disp('Not a color image');
    return;
end

%imshow(original_image);

L = rgb2gray(original_image);
%imshow(L);
%waitforbuttonpress;

I = im2double(L);
I = log(1 + I);

M = 2*size(I,1) + 1;
N = 2*size(I,2) + 1;

sigma = 10;

[X, Y] = meshgrid(1:N,1:M);
centerX = ceil(N/2);
centerY = ceil(M/2);
gaussianNumerator = (X - centerX).^2 + (Y - centerY).^2;
H = exp(-gaussianNumerator./(2*sigma.^2));
H = 1 - H;
H1 = H;

%An Algorithm for Repairing Low-Quality Video Enhancement Techniques Based on Trained Filter

H = fftshift(H);

If = fft2(I, M, N);
Iout = real(ifft2(H.*If));
Iout = Iout(1:size(I,1),1:size(I,2));

Ihmf = exp(Iout)-1;

figure('Name', 'AAA');
subplot(2,2,1)
imshow(original_image);
subplot(2,2,2)
imshow(L);
subplot(2,2,3)
imshow(H1);
subplot(2,2,4)
imshow(Ihmf);

waitforbuttonpress;








