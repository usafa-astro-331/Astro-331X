clearvars; clc;
%
%
load("SUN.mat")

% Data Adjustments
figure(1); clf; hold on
plot(SUN.t/1000, SUN.sun*180/pi)
ylabel('Heading (Deg)')

yyaxis right
plot(SUN.t/1000, SUN.w)
legend('Heading', 'Angular Rate')
xlabel('Time (s)')
ylabel('w (RPM)')
saveas(gcf, 'heading_and_speed.png')