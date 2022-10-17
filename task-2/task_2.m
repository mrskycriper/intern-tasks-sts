%Задаем массивы данных:
x = [0; 0.1; 0.2; 0.3; 0.5; 0.7; 0.9];
y = [3;4.5;1.7;0.7;-1; -2; -6];

%Строим матрицу матрицу Вандермонда W с вырезанным первым столбцом:
W = vander(x);
W = W(1:7,2:7);

%Вычисляем элементы матрицы А как произведение транспонированной матрицы
%(W) и просто матрицы W
A = W'*W;

%Вычисляем элементы вектора b
b = W'*y;

%Решая систему уравнений Aa = b, находим значения вектор-столбца a:
a = A\b;

%Проверяем, используя методы MATLAB = функцию polyfit:
poly_a = polyfit(x,y,5);

%Получаем аппроксимированные значения y:
x1 = -0.1:0.001:1.0;
y1 = a(1)*x1.^5 + a(2)*x1.^4 + a(3)*x1.^3 + a(4)*x1.^2 + a(5)*x1 + a(6);
y2 = poly_a(1)*x1.^5 + poly_a(2)*x1.^4 + poly_a(3)*x1.^3 + poly_a(4)*x1.^2 + poly_a(5)*x1 + poly_a(6);


% Create data and 2-by-1 tiled chart layout
tiledlayout(1,2)

% Top plot
ax1 = nexttile;
plot(ax1,x,y,'*');
hold on;
grid on;
plot(ax1,x1,y1,'Color','g')
title(ax1,'Manual approximation')

% Bottom plot
ax2 = nexttile;
plot(ax2,x,y,'*');
hold on;
grid on;
plot(ax2,x1,y2,'Color','r')
title(ax2,'Polyfit approximation')