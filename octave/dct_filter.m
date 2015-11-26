%DCT

%http://www.mathworks.com/help/images/ref/dct2.html

pkg load image

RGB = imread("../src/images/img001.jpg");
I = rgb2gray(RGB);
J = dct2(I);
figure('Name','Grafico','Numbertitle','off'), imshow(log(abs(J)),[]), colormap(jet(64)), colorbar
J(abs(J) < 10)=0;
K = idct2(J);
figure('Name','Original','Numbertitle','off'), imshow(I);
figure('Name','DCT','Numbertitle','off'), imshow(K,[0 255]);


waitforbuttonpress;
