
/**********************************************************/
/* lh5810 CPU emulation class                            */
/**********************************************************/

#ifndef LH5810_H
#define LH5810_H

#include <QFile>

#include "pobject.h"

#define PB7	((lh5810.r_opb & 0x80)?TRUE:FALSE)

#define SETREG_LH5810_RESET(p,data)	(p->lh5810.reset	= (data));
#define SETREG_LH5810_U(p,data)		(p->lh5810.r_u	= (data));
#define SETREG_LH5810_L(p,data)		(p->lh5810.r_l	= (data));
#define SETREG_LH5810_G(p,data)		(p->lh5810.r_g	= (data));
#define SETREG_LH5810_MSK(p,data)	(p->lh5810.r_msk = (data)&0x0F);
#define SETREG_LH5810_IF(p,data)	(p->lh5810.r_if	= (data));
#define SETREG_LH5810_DDA(p,data)	(p->lh5810.r_dda = (data));
#define SETREG_LH5810_DDB(p,data)	(p->lh5810.r_ddb = (data));
#define SETREG_LH5810_OPA(p,data)	(p->lh5810.r_opa = ( (p->lh5810.r_opa & (~p->lh5810.r_dda)) | ((data) & (p->lh5810.r_dda))) );
#define SETREG_LH5810_OPB(p,data)	(p->lh5810.r_opb = ( (p->lh5810.r_opb & (~p->lh5810.r_ddb)) | ((data) & (p->lh5810.r_ddb))) );
#define SETREG_LH5810_OPC(p,data)	(p->lh5810.r_opc = (data));
#define SETREG_LH5820_F(p,data)		(p->lh5810.r_f	= (data));




class CLH5810:public CPObject{
public:
    enum LH5810_REGS{
        RESET,
        U,
        L,
        G,
        MSK,
        IF,
        DDA,
        DDB,
        OPA,
        OPB,
        OPC,
        F
    };

    typedef struct
    {
        UINT8 reset,r_g,r_u,r_l,r_msk,r_if,r_dda,r_ddb,r_opa,r_opb,r_opc,r_f;
    } LH5810REG;

    virtual bool	init(void);						//initialize
	bool	exit(void);						//end
	void	Reset(void);
	virtual bool	step(void);

    void	Load_Internal(QFile *);
    void	save_internal(QFile *);
    void	Load_Internal(QXmlStreamReader *);
    void	save_internal(QXmlStreamWriter *);

	void	Regs_Info(UINT8);

    const char*	GetClassName(){ return("CLH5810");}

    UINT8	GetReg(LH5810_REGS reg)
    {
        UINT8 t=0;

        switch (reg)
        {
        case U:		return(lh5810.r_u);
        case L:		return(lh5810.r_l);
        case G:		return(lh5810.r_g);
        case MSK:	t=(lh5810.r_msk);
                            if (IRQ)	t|=0x10;
                            if (PB7)	t|=0x20;
                            return(t);
        case IF:		t=(lh5810.r_if);
    //						if (IRQ)	t|=0x01;
    //						if (PB7)	t|=0x02;
                            return(t);
        case DDA:	return(lh5810.r_dda);
        case DDB:	return(lh5810.r_ddb);
        case OPA:	return(lh5810.r_opa & (~lh5810.r_dda));	//OK
        case OPB:	return(lh5810.r_opb & (~lh5810.r_ddb));	//OK
        case OPC:	return(lh5810.r_opc);
        case F:		return(lh5810.r_f);

        }
        return(0);
    }

    UINT8	SetReg(LH5810_REGS reg,UINT8 data)
    {
        switch (reg)
        {
        case RESET:	return(lh5810.reset	= data);	break;
        case U:		return(lh5810.r_u	= data);	break;
        case L:		return(lh5810.r_l	= data);	break;
        case G:		return(lh5810.r_g	= data);	break;
        case MSK:	return(lh5810.r_msk = data&0x0F);	break;
        case IF:	return(lh5810.r_if	= data);	break;
        case DDA:	return(lh5810.r_dda = data);	break;
        case DDB:	return(lh5810.r_ddb = data);	break;
        case OPA:	return(lh5810.r_opa = ( (lh5810.r_opa & (~lh5810.r_dda)) | (data & (lh5810.r_dda))) );	break;
        case OPB:	return(lh5810.r_opb = ( (lh5810.r_opb & (~lh5810.r_ddb)) | (data & (lh5810.r_ddb))) );	break;
        case OPC:	return(lh5810.r_opc = data);	break;
        case F:		return(lh5810.r_f	= data);	break;
        }

        return (0);
    }

    UINT8	SetRegBit(LH5810_REGS reg,UINT8 bit, bool value)
    {
        if (value)
        {
            switch (reg)
            {
            case U:		return(lh5810.r_u	|= (0x01 << bit));	break;
            case L:		return(lh5810.r_l	|= (0x01 << bit));	break;
            case G:		return(lh5810.r_g	|= (0x01 << bit));	break;
            case MSK:	return(lh5810.r_msk |= (0x01 << bit));	break;
            case IF:	return(lh5810.r_if	|= (0x01 << bit));	break;
            case DDA:	return(lh5810.r_dda |= (0x01 << bit));	break;
            case DDB:	return(lh5810.r_ddb |= (0x01 << bit));	break;
            case OPA:	return(lh5810.r_opa |= (0x01 << bit));	break;
            case OPB:	return(lh5810.r_opb |= (0x01 << bit));	break;
            case OPC:	return(lh5810.r_opc |= (0x01 << bit));	break;
            case F:		return(lh5810.r_f	|= (0x01 << bit));	break;
            default:    break;
            }
        }
        else
        {
            switch (reg)
            {
    #if 1
            case U:		return(lh5810.r_u	&= ((0x01 << bit) ^ 0xff));	break;
            case L:		return(lh5810.r_l	&= ((0x01 << bit) ^ 0xff));	break;
            case G:		return(lh5810.r_g	&= ((0x01 << bit) ^ 0xff));	break;
            case MSK:	return(lh5810.r_msk &= ((0x01 << bit) ^ 0xff));	break;
            case IF:	return(lh5810.r_if	&= ((0x01 << bit) ^ 0xff));	break;
            case DDA:	return(lh5810.r_dda &= ((0x01 << bit) ^ 0xff));	break;
            case DDB:	return(lh5810.r_ddb &= ((0x01 << bit) ^ 0xff));	break;
            case OPA:	return(lh5810.r_opa &= ((0x01 << bit) ^ 0xff));	break;
            case OPB:	return(lh5810.r_opb &= ((0x01 << bit) ^ 0xff));	break;
            case OPC:	return(lh5810.r_opc &= ((0x01 << bit) ^ 0xff));	break;
            case F:		return(lh5810.r_f	&= ((0x01 << bit) ^ 0xff));	break;
            default:    break;
    #else
            case LH5810_U:		return(lh5810.r_u	&= ~(0x01 << bit));	break;
            case LH5810_L:		return(lh5810.r_l	&= ~(0x01 << bit));	break;
            case LH5810_G:		return(lh5810.r_g	&= ~(0x01 << bit));	break;
            case LH5810_MSK:	return(lh5810.r_msk &= ~(0x01 << bit));	break;
            case LH5810_IF:		return(lh5810.r_if	&= ~(0x01 << bit));	break;
            case LH5810_DDA:	return(lh5810.r_dda &= ~(0x01 << bit));	break;
            case LH5810_DDB:	return(lh5810.r_ddb &= ~(0x01 << bit));	break;
            case LH5810_OPA:	return(lh5810.r_opa &= ~(0x01 << bit));	break;
            case LH5810_OPB:	return(lh5810.r_opb &= ~(0x01 << bit));	break;
            case LH5810_OPC:	return(lh5810.r_opc &= ~(0x01 << bit));	break;
            case LH5820_F:		return(lh5810.r_f	&= ~(0x01 << bit));	break;
    #endif
            }
        }
        return (0);
    }


	LH5810REG	lh5810;
	char	Regs_String[255];
	bool	New_L;
	bool	IRQ,INT;
//	UINT8	OPA,OPB;

	CLH5810(CPObject *parent);
    virtual ~CLH5810();

private:
};



#endif

