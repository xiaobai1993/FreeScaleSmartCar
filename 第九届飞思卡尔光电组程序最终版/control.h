void speed_pid(void);
void speed_pid_control_out(void);
void steer_pd(void);
void steer_pd_control_out(void);
//int Pixel_Analysis(unsigned char *Pixel);
void CCD_P(unsigned char *A);
float Calculate_line(void);
void CCD_P2(unsigned char *s,int interval,int yuzhi);
int DirectionControl(void) ;
void find(unsigned char *s,int width,int interval,int yuzhi);
void update_p(struct p *f,int n);
char analys(struct p *f);
int start_check(unsigned char *A,int interval,int yuzhi);

