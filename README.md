# th\_encode\_erase

    #import "1.1/th_dec_ctx.h"
    
    #define CLIP(X) ((X)>0xFF?0xFF:(X)<0?0:X)

    // RGB -> YUV
    #define RGB2Y(R,G,B) CLIP((( 66*(R)+129*(G)+ 25*(B)+128)>>8)+ 16)
    #define RGB2U(R,G,B) CLIP(((-38*(R)- 74*(G)+112*(B)+128)>>8)+128)
    #define RGB2V(R,G,B) CLIP(((112*(R)- 94*(G)- 18*(B)+128)>>8)+128)

    void th_encode_erase(th_dec_ctx *tcx) {
            
        int fw = tcx->state.info.frame_width;
        int fh = tcx->state.info.frame_height;
	
        /*
        OC_FRAME_GOLD (0)
        OC_FRAME_PREV (1)
        OC_FRAME_SELF (2)
        OC_FRAME_IO (3)
        */
            
        for(int k=0; k<2; k++) {
            
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
                        
                        *y=RGB2Y(0,0,0xFF);
                        *u=RGB2U(0,0,0xFF);
                        *v=RGB2V(0,0,0xFF);
                        
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
		
