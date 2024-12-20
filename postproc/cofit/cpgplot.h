#ifndef cpgplot_h
#define cpgplot_h

typedef int Logical;

void cpgarro(float x1, float y1, float x2, float y2);
void cpgask(Logical flag);
int cpgband(int mode, int posn, float xref, float yref, float *x, float *y, char *ch_scalar);
void cpgbbuf(void);
int cpgbeg(int unit, char *file, int nxsub, int nysub);
void cpgbin(int nbin, float *x, float *data, Logical center);
void cpgbox(char *xopt, float xtick, int nxsub, char *yopt, float ytick, int nysub);
void cpgcirc(float xcent, float ycent, float radius);
void cpgconb(float *a, int idim, int jdim, int i1, int i2, int j1, int j2, float *c, int nc, float *tr, float blank);
void cpgconl(float *a, int idim, int jdim, int i1, int i2, int j1, int j2, float c, float *tr, char *label, int intval, int minint);
void cpgcons(float *a, int idim, int jdim, int i1, int i2, int j1, int j2, float *c, int nc, float *tr);
void cpgcont(float *a, int idim, int jdim, int i1, int i2, int j1, int j2, float *c, int nc, float *tr);
void cpgctab(float *l, float *r, float *g, float *b, int nc, float contra, float bright);
int cpgcurs(float *x, float *y, char *ch_scalar);
void cpgdraw(float x, float y);
void cpgebuf(void);
void cpgend(void);
void cpgenv(float xmin, float xmax, float ymin, float ymax, int just, int axis);
void cpgeras(void);
void cpgerrb(int dir, int n, float *x, float *y, float *e, float t);
void cpgerrx(int n, float *x1, float *x2, float *y, float t);
void cpgerry(int n, float *x, float *y1, float *y2, float t);
void cpgetxt(void);
void cpggray(float *a, int idim, int jdim, int i1, int i2, int j1, int j2, float fg, float bg, float *tr);
void cpghi2d(float *data, int nxv, int nyv, int ix1, int ix2, int iy1, int iy2, float *x, int ioff, float bias, Logical center, float *ylims);
void cpghist(int n, float *data, float datmin, float datmax, int nbin, int pgflag);
void cpgiden(void);
void cpgimag(float *a, int idim, int jdim, int i1, int i2, int j1, int j2, float a1, float a2, float *tr);
void cpglab(char *xlbl, char *ylbl, char *toplbl);
void cpglcur(int maxpt, int *npt, float *x, float *y);
void cpgldev(void);
void cpglen(int units, char *string, float *xl, float *yl);
void cpgline(int n, float *xpts, float *ypts);
void cpgmove(float x, float y);
void cpgmtxt(char *side, float disp, float coord, float fjust, char *text);
void cpgncur(int maxpt, int *npt, float *x, float *y, int symbol);
void cpgnumb(int mm, int pp, int form, char *string, int *string_length);
void cpgolin(int maxpt, int *npt, float *x, float *y, int symbol);
void cpgpage(void);
void cpgpanl(int nxc, int nyc);
void cpgpap(float width, float aspect);
void cpgpixl(int *ia, int idim, int jdim, int i1, int i2, int j1, int j2, float x1, float x2, float y1, float y2);
void cpgpnts(int n, float *x, float *y, int *symbol, int ns);
void cpgpoly(int n, float *xpts, float *ypts);
void cpgpt(int n, float *xpts, float *ypts, int symbol);
void cpgptxt(float x, float y, float angle, float fjust, char *text);
void cpgqah(int *fs, float *angle, float *vent);
void cpgqcf(int *font);
void cpgqch(float *size);
void cpgqci(int *ci);
void cpgqcir(int *icilo, int *icihi);
void cpgqcol(int *ci1, int *ci2);
void cpgqcr(int ci, float *cr, float *cg, float *cb);
void cpgqcs(int units, float *xch, float *ych);
void cpgqfs(int *fs);
void cpgqinf(char *item, char *value, int *value_length);
void cpgqitf(int *itf);
void cpgqls(int *ls);
void cpgqlw(int *lw);
void cpgqpos(float *x, float *y);
void cpgqtbg(int *tbci);
void cpgqtxt(float x, float y, float angle, float fjust, char *text, float *xbox, float *ybox);
void cpgqvp(int units, float *x1, float *x2, float *y1, float *y2);
void cpgqvsz(int units, float *x1, float *x2, float *y1, float *y2);
void cpgqwin(float *x1, float *x2, float *y1, float *y2);
void cpgrect(float x1, float x2, float y1, float y2);
float cpgrnd(float x, int *nsub);
void cpgrnge(float x1, float x2, float *xlo, float *xhi);
void cpgsah(int fs, float angle, float vent);
void cpgsave(void);
void cpgunsa(void);
void cpgscf(int font);
void cpgsch(float size);
void cpgsci(int ci);
void cpgscir(int icilo, int icihi);
void cpgscr(int ci, float cr, float cg, float cb);
void cpgscrn(int ci, char *name, int *ier);
void cpgsfs(int fs);
void cpgshls(int ci, float ch, float cl, float cs);
void cpgsitf(int itf);
void cpgsls(int ls);
void cpgslw(int lw);
void cpgstbg(int tbci);
void cpgsubp(int nxsub, int nysub);
void cpgsvp(float xleft, float xright, float ybot, float ytop);
void cpgswin(float x1, float x2, float y1, float y2);
void cpgtbox(char *xopt, float xtick, int nxsub, char *yopt, float ytick, int nysub);
void cpgtext(float x, float y, char *text);
void cpgupdt(void);
void cpgvect(float *a, float *b, int idim, int jdim, int i1, int i2, int j1, int j2, float c, int nc, float *tr, float blank);
void cpgvsiz(float xleft, float xright, float ybot, float ytop);
void cpgvstd(void);
void cpgwedg(char *side, float disp, float width, float fg, float bg, char *label);
void cpgwnad(float x1, float x2, float y1, float y2);

#endif
