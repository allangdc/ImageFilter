%SVLM
clear all;
close all;
tic;
original_image = imread('/home/allan/Imagens/floresta.jpg');
image_size = size(original_image);
height = image_size(1,1);
width = image_size(1,2);
if(length(image_size) == 3)
    R = double(original_image(:,:,1));
    G = double(original_image(:,:,2));
    B = double(original_image(:,:,3));
    I = 0.299*R + 0.587*G + 0.114*B;
else
    disp('Not a color image');
    return;
end
% figure('Name','Original Image','Numbertitle','off');
% imshow(original_image);

% figure('Name','Luminance','Numbertitle','off');
% imshow(mat2gray(I));

gaussian_lpf = fspecial('gaussian',[3, 3]);
L1 = imfilter(I,gaussian_lpf);
L2 = imresize(imfilter(imresize(I,[height/2, width/2]),gaussian_lpf),[height, width],'bicubic');
L3 = imresize(imfilter(imresize(I,[height/4, width/4]),gaussian_lpf),[height, width],'bicubic');
L4 = imresize(imfilter(imresize(I,[height/8, width/8]),gaussian_lpf),[height, width],'bicubic');

SVLM = (L1 + L2 + L3 + L4)/4;
% figure('Name','SVLM','Numbertitle','off');
% imshow(mat2gray(SVLM));


alfa = 0.5;
gamma = double(alfa.^((128 - double(SVLM))/128));
O = 255*((double(I)/255).^gamma);
% figure('Name','Enhanced Luminance','Numbertitle','off');
% imshow(mat2gray(O));

sigma = std2(I);
if sigma <= 40
    P = 2;
else if sigma > 40 && sigma <= 80
        P = ((-0.025*sigma) + 3);
    else
        P = 1;
    end
end

E = (double(SVLM)./O).^double(P);
S = 255*((O./255).^E);
% figure('Name','Enhanced Contrast','Numbertitle','off');
% imshow(mat2gray(S));

lambda_red = 0.9;
lambda_green = 0.9;
lambda_blue = 0.9;
Rm = S.*(R./O)*lambda_red;
Gm = S.*(G./O)*lambda_green;
Bm = S.*(B./O)*lambda_blue;

enhanced_image = cat(3,Rm,Gm,Bm);
toc;
figure('Name','Enhanced Image','Numbertitle','off');
imshow(enhanced_image/255);

% tic;
% corrected_image = enhanced_image;
% for y=1:100
%     for x=1:100
%         r = enhanced_image(y,x,1);
%         g = enhanced_image(y,x,2);
%         b = enhanced_image(y,x,3);
%         [r_o,g_o,b_o] = color_correction(r,g,b);        
%         corrected_image(y,x,1) = r_o*255; 
%         corrected_image(y,x,2) = g_o*255;
%         corrected_image(y,x,3) = b_o*255;
%     end
% end
% toc;
% figure('Name','Test');
% imshow(corrected_image);