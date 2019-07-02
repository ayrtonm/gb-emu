x=load("dist");
#idx=find(x<3);
#x=x(idx);
#figure(1,'visible','off')
[n, f] = hist(x,50000);
#axis([0 10 0 100])
set(gca,'xscale','log')
#set(gca,'yscale','log')
set(gca,'fontsize',14)
grid on
xlabel('measured time between opcodes [\mus]','fontsize',18)
ylabel('number of events','fontsize',18)
xm = mean(x);
xs = std(x);
title(sprintf("mean = %f, std dev = %f", xm, xs));
print -dpng "nothrottle.png"
