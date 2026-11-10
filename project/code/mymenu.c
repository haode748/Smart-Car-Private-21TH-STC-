#include "zf_common_headfile.h"
#include "ALL.h"
#include "menu.h"
#include "mymenu.h"

extern float encoder_date1;
extern float encoder_date2;

Menu_Item head;
Menu_Item *Key;

Menu_Item m1, m2, m3, m4, m5, m6, m7, m8;

float number1 = 0.0f;


void menu_Init(PID_t *pid1, PID_SERVO *pid2)
{
    Menu_Item *motor;
    Menu_Item *servo;

    head.variable = NULL;
    head.father = NULL;
    head.first_son = NULL;
    head.kind = Menu_Folder;
    head.last_brother = NULL;
    head.name = "MENU";
    head.next_brother = NULL;
    head.No = 0;
    head.sons = 0;
    head.select = 0;

    motor = dynamicCreate_Menu_Folder(&head, "motor");
    servo = dynamicCreate_Menu_Folder(&head, "servo");

        Create_Menu_Number(motor, &m1, "Kp", &pid1->Kp, float_Box);
        Create_Menu_Number(motor, &m2, "Ki", &pid1->Ki, float_Box);
        Create_Menu_Number(motor, &m3, "Kd", &pid1->Kd, float_Box);
    
        Create_Menu_Number(servo, &m4, "Kp", &pid2->kp1, float_Box);
        Create_Menu_Number(servo, &m5, "Ki", &pid2->kp2, float_Box);
        Create_Menu_Number(servo, &m6, "Kd", &pid2->kd1, float_Box);
  

    Key = head.first_son;
}

/* 显示光标 */
void Show_Key(void)
{
    Menu_Item *h;
    Menu_Item *s;
    int i;

    if(Key == NULL) return;
    if(Key->father == NULL) return;

    h = Key->father;
    s = h->first_son;

    for(i = 0; i < h->sons; i++)
    {
        if(s == Key)
        {
            ips200_show_string(0, i * 15, "->");
        }
        else
        {
            ips200_show_string(0, i * 15, "  ");
        }

        if(s != NULL)
        {
            s = s->next_brother;
        }
    }
}

/* 显示数据 */
void Show_Number(void)
{
    Menu_Item *h;
    Menu_Item *s;
    int i;

    if(Key == NULL) return;
    if(Key->father == NULL) return;

    h = Key->father;
    s = h->first_son;

    for(i = 0; i < h->sons; i++)
    {
        if(s == NULL) break;

        if(s->select)
        {
            ips200_show_char(74, i * 15, '[');
        }
        else
        {
            ips200_show_char(74, i * 15, ' ');
        }

        switch(s->kind)
        {
            case int32_Box:
            {
                ips200_show_int32(86, i * 15, *(int32 *)s->variable, 5);
                break;
            }

            case float_Box:
            {
                ips200_show_float(86, i * 15, *(float *)s->variable, 3, 1);
                break;
            }

            default:
                break;
        }

        s = s->next_brother;
    }
}

/* 显示菜单 */
void menu_show(void)
{
    Menu_Item *h;
    Menu_Item *s;
    int i;

    if(Key == NULL) return;
    if(Key->father == NULL) return;

    h = Key->father;
    s = h->first_son;

    for(i = 0; i < h->sons; i++)
    {
        if(s == NULL) break;
        ips200_show_string(20, i * 15, s->name);
        s = s->next_brother;
    }

    Show_Number();
    Show_Key();
}

/* 向下 */
void Key_down(void)
{
    if(Key == NULL) return;

    if(Key->next_brother != NULL)
    {
        Key = Key->next_brother;
    }
    else
    {
        if(Key->father != NULL && Key->father->first_son != NULL)
        {
            Key = Key->father->first_son;
        }
    }
}

/* 进入 */
void Key_enter(void)
{
    if(Key == NULL) return;

    if(Key->sons > 0 && Key->first_son != NULL)
    {
        Key = Key->first_son;
        ips200_clear(RGB565_WHITE);
    }
}

/* 退出 */
void Key_qiut(void)
{
    if(Key == NULL) return;
    if(Key->father == NULL) return;

    if(Key->father->father != NULL)
    {
        if(Key->select == 0)
        {
            Key = Key->father;
            ips200_clear(RGB565_WHITE);
        }
    }
}

/* 参数选择 */
void Key_select(void)
{
    if(Key == NULL) return;

    if(Key->kind != Menu_Folder)
    {
        Key->select = !Key->select;
    }
}

/* 选择 */
void Key_2(void)
{
    if(Key == NULL) return;

    if(Key->select == 0)
    {
        Key_down();
    }
}

/* 编码器控制参数增加减少 */
void Key_variable_plusORsub_1(void)
{
    if(Key == NULL) return;
	
	
    if(Key->select)
    {
        if((float)encoder_get_count(PWMA_ENCODER)>512)
        {
            number1 += 1.0f;
        }
        else if((float)encoder_get_count(PWMA_ENCODER)<-512)
        {
            number1 -= 1.0f;
        }
		else
			number1+=0.0;

        if((float)encoder_get_count(PWMC_ENCODER)>512)
        {
            number1 += 0.1f;
        }
        else if((float)encoder_get_count(PWMC_ENCODER)<-512)
        {
            number1 -= 0.1f;
        }
		else
			number1+=0.0;

        if(Key->variable != NULL)
        {
            switch(Key->kind)
            {
                case int32_Box:
                    *(int32 *)Key->variable += (int32)number1;
                    break;

                case float_Box:
                    *(float *)Key->variable += number1; 
//				    printf("%f,%f\n",*(float *)Key->variable,encoder_date1);
                    break;

                default:
                    break;
            }
        }
       
//        number1 = 0.0f;
    }
}

/* 进入或选择参数 */
void Key_3(void)
{
    if(Key == NULL) return;

    switch(Key->kind)
    {
        case Menu_Folder:
            Key_enter();
            break;

        default:
            Key_select();
            break;
    }
}
