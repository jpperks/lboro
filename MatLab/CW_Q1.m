close all
clear all
% Variable Definitions
bn = (100*10^-6);
bp = (42*10^-6);
vtn = 0.3;
vtp = -0.3;
vdd = 1;
cload = 5*10^-15;
dt = 0.01;
vdsn = 0:dt:1;
vdsp = -(0:dt:1);
vgsn = 0.2:0.2:1.0;
vgsp = -0.2:-0.2:-1.0;
idsn = zeros(length(vdsn),5);
idsp = zeros(length(vdsn),5);
i=1;
% nFET Calculation
for m=1:length(vgsn),
        for n=1:length(vdsn),
            if vdsn(n)<=(vgsn(m)-vtn),
                idsn(n,i) = bn*((vgsn(m)-vtn)*vdsn(n)-((vdsn(n)^2)/2));
            end
            if vdsn(n)>(vgsn(m)-vtn),
                idsn(n,i) = bn/2*((vgsn(m)-vtn)^2);
            end
            if vgsn(m)<=vtn,
                idsn(n,i) = 0;  
            end
        end
    subplot(2,1,1)
    plot(vdsn,idsn(:,i),'r-')
    title('nFET IV Curve')
    xlabel('VDS (Volts)')
    ylabel('IDS (Amps)')
    hold on;
    i=i+1;
end
% pFET Calculation  
for m=1:length(vgsp),
        for n=1:length(vdsp),
            if vdsp(n)>=(vgsp(m)-vtp),
                idsp(n,i) = -bp*((vgsp(m)-vtp)*vdsp(n)-((vdsp(n)^2)/2));
            end
            if vdsp(n)<(vgsp(m)-vtp),
                idsp(n,i) = -bp/2*((vgsp(m)-vtp)^2);
            end
            if vgsp(m)>=vtp,
                idsp(n,i) = 0;  
            end
        end
    subplot(2,1,2)
    plot(vdsp,idsp(:,i),'g-')
    title('pFET IV Curve')
    xlabel('VDS (Volts)')
    ylabel('IDS (Amps)')
    hold on;
    i=i+1;
end
