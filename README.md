# th\_encode\_erase

Clear ref\_frame\_bufs of [theora-1.1](https://github.com/xiph/theora/tree/theora-1.1)

    #import "th_dec_ctx.h"
    
    void th_encode_erase(th_dec_ctx *tcx) {
            
        int fw = tcx->state.info.frame_width;
        int fh = tcx->state.info.frame_height;
            
        for(int k=0; k<4; k++) {
            
            int framei = tcx->state.ref_frame_idx[k];
            if(tcx->state.ref_frame_bufs[framei][0].width==fw&&tcx->state.ref_frame_bufs[framei][0].height==fh) {
                
                unsigned char *y_row= tcx->state.ref_frame_bufs[framei][0].data;
                unsigned char *u_row= tcx->state.ref_frame_bufs[framei][1].data;
                unsigned char *v_row= tcx->state.ref_frame_bufs[framei][2].data;
                int y_stride= tcx->state.ref_frame_bufs[framei][0].stride;
                int u_stride= tcx->state.ref_frame_bufs[framei][1].stride;
                int v_stride= tcx->state.ref_frame_bufs[framei][2].stride;
                
                for(int i=0; i<fh; i++) {
                    
                    int dc;
                    
                    unsigned char *y=y_row;
                    unsigned char *u=u_row;
                    unsigned char *v=v_row;
                    
                    for(int j=0; j<fw; j++) {
                        
                        *y=0x80;
                        *u=0x80;
                        *v=0x80;
                        
                        dc=(y-y_row&1)|(tcx->state.info.pixel_fmt&1);
                        
                        y++;
                        u+=dc;
                        v+=dc;
                    }
                    
                    dc=-((i&1)|tcx->state.info.pixel_fmt>>1);
                    y_row+=y_stride;
                    u_row+=dc&u_stride;
                    v_row+=dc&v_stride;
                }
            }
        }
    }
		