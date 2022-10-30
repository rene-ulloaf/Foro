#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

const char _TRUE = 1;
const char _FALSE = 0;
const char *ARCHIVOUSER = "usuarios.txt";
const char *FOLDERTEMAS = "tema/"/*"C:/temas/";*/;
const char *ARCHIVOTEMA = "temas.txt";
const char *ARCHIVOCANTTEMA = "cant_temas.txt";
const short int _MAXNOMBREUSER = 25;
const short int _MAXPASS = 10;
const short int _MAXTEMA = 100;
const short int _MAXCOMEN = 1000;

struct datos{
    char *nombre;
    char *pais;
    char *ip;
};
struct temas{
    char usuario[25];
    char tema[100];
    char fecha[8];
    char hora[8];
    int vigente;
};

void MenuPrincipal(char);
void MenuRegistrado(char);
void Registro();
void InicioSesion();
char RegistroOK(char *,char *,char *);
char ExisteUsuario(char *);
char EsUsuario(char *,char *);
char CrearUsuario(char *,char *);
void DatosUsuario(char *);
void VerTemas();
void VerTema(char *,char *, char *, char *);
void CrearTema();
char nuevoComentario(char *);
void QuitaEspacios(char **);
void ObtenerFechaFormateada(char **);
void ObtenerIP(char **);
void Salir();

struct datos d;

int main(){
    MenuPrincipal(0);
    //system("pause");
}

void MenuPrincipal(char op){
    char *sel=NULL;

    sel = (char *) malloc(sizeof(char)* 1);
    printf(" Foro en C\n =========\n");
    if(op){
        printf("\nOpcion no permitida\n");
    }
    printf("[1] Registrarse\n");
    printf("[2] Iniciar Sesion\n");
    printf("[3] Salir");
    printf("\n\nOpción : ");
    scanf("%s", sel);
    
    if(!strcmp(sel,"1")){
        system("clear");
        Registro();
    }else if(!strcmp(sel,"2")){
        InicioSesion();
    }else if(!strcmp(sel,"3")){
        Salir();
    }else{
        system("clear");
        MenuPrincipal(_TRUE);
    }

    /*switch(sel){
        case 1:
            Registro();
        break;
        case 2:
            InicioSesion();
        break;
        case 3:
            Salir();
        break;
        default:
            MenuPrincipal(_TRUE);
     }*/
}

void MenuRegistrado(char op){
    char *sel=NULL;
    
    sel = (char *) malloc(sizeof(char)* 1);
    printf("Foro");
    printf("\tUsuario : %s", d.nombre);
    /*printf("\tPaís : ");
    printf("\tIP : ");*/
    printf("\n=============================\n");
    if(op){printf("\nOpcion no permitida\n");}
    printf("[1] Ver los temas \n");
    printf("[2] Crear un nuevo tema \n");
    printf("[3] Cerrar Sesion \n");
    printf("\n\nOpción : ");
    scanf("%s", sel);

    if(!strcmp(sel,"1")){
        VerTemas();
    }else if(!strcmp(sel,"2")){
        CrearTema();
    }else if(!strcmp(sel,"3")){
        d.nombre = "";
        system("clear");
        MenuPrincipal(_FALSE);
    }else{
        system("clear");
        MenuRegistrado(_TRUE);
    }
    /*switch(sel){
        case 1:
            VerTemas();
        break;
        case 2:
            CrearTema();
        break;
        case 3:
            system("clear");
            MenuPrincipal(_FALSE);
        break;
        default:
            system("clear");
            MenuRegistrado(_TRUE);
    }*/
}

void Registro(){
    char *nombreUsuario=NULL,*pass1=NULL,*pass2=NULL;

    nombreUsuario = (char *) malloc(sizeof(char)* _MAXNOMBREUSER);
    pass1 = (char *) malloc(sizeof(char)* _MAXPASS);
    pass2 = (char *) malloc(sizeof(char)* _MAXPASS);

    printf("Registro Nuevo Usuario\n=========================\n");
    printf("\nNombre de Usuario : ");
    scanf("%s", nombreUsuario);
    printf("\nClave : ");
    scanf("%s", pass1);
    printf("\nConfirmar Clave : ");
    scanf("%s", pass2);

    system("clear");
    if(!RegistroOK(nombreUsuario,pass1,pass2)){
        if(CrearUsuario(nombreUsuario,pass1)){
            DatosUsuario(nombreUsuario);
            system("clear");
            MenuRegistrado(_FALSE);
        }
    }else{
        Registro();
    }

    free(nombreUsuario);
    free(pass1);
    free(pass2);
}

void InicioSesion(){
    char *nombreUsuario=NULL,*pass=NULL;

    nombreUsuario = (char *) malloc(sizeof(char)* _MAXNOMBREUSER);
    pass = (char *) malloc(sizeof(char)* _MAXPASS);

    system("clear");
    printf("Inicio Sesión\n=========================\n");
    printf("\nNombre de Usuario : ");
    scanf("%s", nombreUsuario);
    printf("\nClave : ");
    scanf("%s", pass);

    if(EsUsuario(nombreUsuario,pass)){
        DatosUsuario(nombreUsuario);
        system("clear");
        MenuRegistrado(_FALSE);
    }else{
        system("clear");
        printf("Nombre de Usuario y/o contraseña no válidos\n\n");
        free(nombreUsuario);
        free(pass);
        //InicioSesion();
        MenuPrincipal(_FALSE);
    }
}

char RegistroOK(char *nombreUsuario,char *pass1,char *pass2){
    char _Error = _FALSE;

    if(!strcmp("",nombreUsuario)){
        printf("\nNombre de Usuario vacio\n");
        _Error = _TRUE;
    }

    if(!(strcmp("",pass1) || strcmp("",pass2))){
        printf("\nClave vacia\n");
        _Error = _TRUE;
    }

    if(strcmp(pass1,pass2)){
        printf("\nClaves Distintas\n");
        _Error = _TRUE;
    }

    if(ExisteUsuario(nombreUsuario)){
        printf("\nNombre de usuario Existe\n\n");
        _Error = _TRUE;
    }

    return _Error;
}

/*int CantUsuarios(){
    int cantUser=0;
    FILE *archivo;

    archivo = fopen(ARCHIVOUSER,"r");
    if(archivo != NULL){
        while(!feof(archivo)){
            cantUser = fgetc(archivo);
        }
    }
    fclose(archivo);

    return cantUser;
}*/

char ExisteUsuario(char *nombreUsuario){
    char ret=_FALSE,*user=NULL;
    FILE *archivo;

    user = (char *) malloc(sizeof(char)* _MAXNOMBREUSER);
    archivo = fopen(ARCHIVOUSER,"r");

    if(archivo != NULL){
        while(!feof(archivo) && fread(user,1,1,archivo)){
            fseek(archivo,-1, SEEK_CUR);
            fscanf(archivo,"%s",user);

            if(!strcmp(user,nombreUsuario)){
                ret = _TRUE;
                fseek(archivo,0,SEEK_END);
            }else{
                fscanf(archivo,"%s",user);
                fscanf(archivo,"%s",user);
                fscanf(archivo,"%s",user);
            }
        }

        fflush(archivo);
        fclose(archivo);
    }

    free(user);
    return ret;
}

char EsUsuario(char *nombreUsuario,char *pass){
    char ret=_FALSE,*user=NULL,*password=NULL,*ruta=NULL;
    FILE *archivo;

    user = (char *) malloc(sizeof(char)* _MAXNOMBREUSER);
    password = (char *) malloc(sizeof(char)* _MAXPASS);
    ruta = (char *) malloc(sizeof(char)* 18);

    strcpy(ruta,"");
    strcat(ruta,FOLDERTEMAS);
    strcat(ruta,ARCHIVOUSER);

    archivo = fopen(ruta,"r");

    if(archivo != NULL){
        while(!feof(archivo) && fread(user,1,1,archivo)){
            fseek(archivo,-1, SEEK_CUR);
            
            fscanf(archivo,"%s",user);
            fscanf(archivo,"%s",password);

            if(strcmp(user,nombreUsuario) == 0 && strcmp(pass,password) == 0){
                ret = _TRUE;
                fseek(archivo,0,SEEK_END);
            }else{
                fscanf(archivo,"%s",user);
                fscanf(archivo,"%s",user);
                //fscanf(archivo,"%s",user);
            }
        }

        fflush(archivo);
        fclose(archivo);
    }
    
    free(user);
    free(password);

    return ret;
}

char CrearUsuario(char *nombreUsuario,char *pass){
    char ret=_FALSE,*ruta=NULL,*fec=NULL;
    FILE *archivo;

    ruta = (char *) malloc(sizeof(char)* 18);
    fec = (char *) malloc(sizeof(char)* 128);

    strcpy(ruta,"");
    strcat(ruta,FOLDERTEMAS);
    strcat(ruta,ARCHIVOUSER);

    archivo = fopen(ruta,"a");
    if(archivo != NULL){
        /*if(ftell(archivo) > 0){
            fputc('\n',archivo);
        }*/
        fputs(nombreUsuario,archivo);
        fputc('\t',archivo);
        fputs(pass,archivo);
        fputc('\t',archivo);
        ObtenerFechaFormateada(&fec);
        fputs(fec,archivo);
        fputc('\n',archivo);

        fflush(archivo);
        fclose(archivo);

        ret = _TRUE;
    }

    //free(fec);
    return ret;
}

void DatosUsuario(char *user){
    char *ip=NULL;
    ip = (char *) malloc(sizeof(char)* 16);

    d.nombre  = user;
    /*d.pais = "";
    ObtenerIP(&ip);
    d.ip = ip;*/
}

void VerTemas(){
    int cantTemas,cont=0,i,aux,muestra=0,ind=0;
    char irTema,*usuario=NULL,*tema=NULL,*auxTema=NULL,*fecha=NULL,*hora=NULL,*vigente=NULL,*ruta=NULL,*op=NULL,*aux2=NULL;
    FILE *archivo,*archCant;

    archCant = fopen(ARCHIVOCANTTEMA,"r");
    if(archCant != NULL){
        cantTemas = fgetc(archCant);

        fflush(archCant);
        fclose(archCant);
    }

    struct temas t[cantTemas];
    ruta = (char *) malloc(sizeof(char)* 15);
    op = (char *) malloc(sizeof(char)* 1);
    aux2 = (char *) malloc(sizeof(char)* 1);

    strcpy(ruta,"");
    strcat(ruta,FOLDERTEMAS);
    strcat(ruta,ARCHIVOTEMA);
    
    archivo = fopen(ruta,"r");
    if(archivo != NULL){
        while(!feof(archivo) && fread(aux2,1,1,archivo)){
            fseek(archivo,-1, SEEK_CUR);

            usuario = (char *) malloc(sizeof(char)* _MAXNOMBREUSER);
            tema = (char *) malloc(sizeof(char)* _MAXTEMA);
            auxTema = (char *) malloc(sizeof(char)* _MAXTEMA);
            fecha = (char *) malloc(sizeof(char)* 8);
            hora = (char *) malloc(sizeof(char)* 8);
            vigente = (char *) malloc(sizeof(char)* 1);

            fscanf(archivo,"%s", usuario);
            fscanf(archivo,"%s", fecha);
            fscanf(archivo,"%s", hora);
            fscanf(archivo,"%s", vigente);
            fgetc(archivo);
            fgets(tema, _MAXTEMA, archivo);

            for(i=0;i<strlen(tema)-1;i++){
                auxTema[i] = tema[i];
            }

            strcpy(t[cont].usuario,usuario);
            strcpy(t[cont].tema,auxTema);
            strcpy(t[cont].fecha,fecha);
            strcpy(t[cont].hora,hora);
            t[cont].vigente = (int) vigente;
            
            cont++;

            usuario = NULL;
            fecha = NULL;
            hora = NULL;
            vigente = NULL;
            tema = NULL;
            auxTema = NULL;
        }

        free(usuario);
        free(tema);
        free(fecha);
        free(hora);
        free(vigente);
        free(ruta);

        fflush(archivo);
        fclose(archivo);

        do{
            aux = cont - 10;
            if(aux <= 0){
                aux += 10;
            }else{
                aux = 10;
            }
            cont -= aux;

            for(i=0;i<=aux-1;i++){
                if(t[i].vigente){
                    printf("\n[%d] %s \n",i+1,t[i].tema);
                    muestra++;
                }else{
                    i--;
                }
            }

            if(cont != 0){
                printf("\n[S]iguientes temas");
            }
            printf("\n[V]olver al Menu\n");

            do{
                printf("\nEscoja Tema : ");
                scanf("%s", op);

                if((strcmp(op,"S") == 0) || (strcmp(op,"s") == 0)){
                    irTema = _TRUE;
                }else if((strcmp(op,"v") == 0) || (strcmp(op,"V") == 0)){
                    system("clear");
                    MenuRegistrado(_FALSE);
                }else if(!strcmp(op,"1")){
                    if(aux >= 1){
                        irTema = _TRUE;
                        if(aux-10 < 0){
                            ind = 1;
                        }else{
                            ind = (aux-10)+1;
                        }
                        cont = 0;
                    }else{
                        irTema = _FALSE;
                        printf("\nTema no Existe\n");
                    }
                }else if(!strcmp(op,"2")){
                    if(aux >= 2){
                        irTema = _TRUE;
                        if(aux-10 < 0){
                            ind = 2;
                        }else{
                            ind = (aux-10)+2;
                        }
                        cont = 0;
                    }else{
                        irTema = _FALSE;
                        printf("\nTema no Existe\n");
                    }
                }else if(!strcmp(op,"3")){
                    if(aux >= 3){
                        irTema = _TRUE;
                        if(aux-10 < 0){
                            ind = 3;
                        }else{
                            ind = (aux-10)+3;
                        }
                        cont = 0;
                    }else{
                        irTema = _FALSE;
                        printf("\nTema no Existe\n");
                    }
                }else if(!strcmp(op,"4")){
                    if(aux >= 4){
                        irTema = _TRUE;
                        if(aux-10 < 0){
                            ind = 4;
                        }else{
                            ind = (aux-10)+4;
                        }
                        cont = 0;
                    }else{
                        irTema = _FALSE;
                        printf("\nTema no Existe\n");
                    }
                }else if(!strcmp(op,"5")){
                    if(aux >= 5){
                        irTema = _TRUE;
                        if(aux-10 < 0){
                            ind = 5;
                        }else{
                            ind = (aux-10)+5;
                        }
                        cont = 0;
                    }else{
                        irTema = _FALSE;
                        printf("\nTema no Existe\n");
                    }
                }else if(!strcmp(op,"6")){
                    if(aux >= 6){
                        irTema = _TRUE;
                        if(aux-10 < 0){
                            ind = 6;
                        }else{
                            ind = (aux-10)+6;
                        }
                        cont = 0;
                    }else{
                        irTema = _FALSE;
                        printf("\nTema no Existe\n");
                    }
                }else if(!strcmp(op,"7")){
                    if(aux >= 7){
                        irTema = _TRUE;
                        if(aux-10 < 0){
                            ind = 7;
                        }else{
                            ind = (aux-10)+7;
                        }
                        cont = 0;
                    }else{
                        irTema = _FALSE;
                        printf("\nTema no Existe\n");
                    }
                }else if(!strcmp(op,"8")){
                    if(aux >= 8){
                        irTema = _TRUE;
                        if(aux-10 < 0){
                            ind = 8;
                        }else{
                            ind = (aux-10)+8;
                        }
                        cont = 0;
                    }else{
                        irTema = _FALSE;
                        printf("\nTema no Existe\n");
                    }
                }else if(!strcmp(op,"9")){
                    if(aux >= 9){
                        irTema = _TRUE;
                        if(aux-10 < 0){
                            ind = 9;
                        }else{
                            ind = (aux-10)+9;
                        }
                        cont = 0;
                    }else{
                        irTema = _FALSE;
                        printf("\nTema no Existe\n");
                    }
                }else if(!strcmp(op,"10")){
                    if(aux >= 10){
                        irTema = _TRUE;
                        if(aux-10 < 0){
                            ind = 10;
                        }else{
                            ind = (aux-10)+10;
                        }
                        cont = 0;
                    }else{
                        irTema = _FALSE;
                        printf("\nTema no Existe\n");
                    }
                }else{
                    irTema = _FALSE;
                    printf("\nTema no Existe\n");
                }
            }while(!irTema);
        }while(cont);
    }else{
        system("clear");
        printf("No Existen Temas.\n\n");
        MenuRegistrado(_FALSE);
    }

    if(irTema){
        VerTema(t[ind-1].usuario,t[ind-1].tema,t[ind-1].fecha,t[ind-1].hora);
    }

    //free(usuario);
}

void VerTema(char *usuario, char *nombreTema, char *fecTema, char *horaTema){
    FILE *archComent;
    char aux=_FALSE,*comentario=NULL,*usuarioComen=NULL,*fecCom=NULL,*horaCom=NULL,*rutaCom=NULL,*archCom=NULL,*op=NULL;

    comentario = (char *) malloc(sizeof(char)* _MAXCOMEN);
    usuarioComen = (char *) malloc(sizeof(char)* _MAXNOMBREUSER);
    fecCom = (char *) malloc(sizeof(char)* 8);
    horaCom = (char *) malloc(sizeof(char)* 8);
    rutaCom = (char *) malloc(sizeof(char)* 113);
    archCom = (char *) malloc(sizeof(char)* 104);
    op = (char *) malloc(sizeof(char)* 1);

    strcpy(archCom,"");
    strcat(archCom,fecTema);
    strcat(archCom,"-");
    strcat(archCom,horaTema);
    strcat(archCom,"-");
    strcat(archCom,nombreTema);
    strcat(archCom,".txt");
    QuitaEspacios(&archCom);

    strcpy(rutaCom,"");
    strcat(rutaCom,FOLDERTEMAS);
    strcat(rutaCom,archCom);

    system("clear");
    printf("%s\tcreado por: %s el %s\n =========\n", nombreTema,usuario,fecTema);

    archComent = fopen(rutaCom,"a+");
    if(archComent != NULL){
        while(!feof(archComent) && fread(usuarioComen,1,1,archComent)){
            fseek(archComent,-1, SEEK_CUR);

            fscanf(archComent,"%s", usuarioComen);
            fscanf(archComent,"%s", fecCom);
            fscanf(archComent,"%s", horaCom);
            fgetc(archComent);
            fgets(comentario, 1000, archComent);

            printf("\n%s dice: \t (creado el %s a las %s)", usuarioComen,fecCom,horaCom);
            printf("\n%s", comentario);
            printf("\n==========================================");
        }

        free(comentario);
        free(usuarioComen);
        free(fecCom);
        free(horaCom);
        free(rutaCom);

        fflush(archComent);
        fclose(archComent);
    }

    printf("\n\n[1] Excribir nuevo Comentario");
    printf("\n\n[2] Volver a los Temas");

    do{
        printf("\n opcion : ");
        scanf("%s", op);

        if(!strcmp(op,"1")){
            aux = _FALSE;
        }else if(!strcmp(op,"2")){
            aux = _FALSE;
        }else{
            printf("\nOpción no válida.\n");
            aux = _TRUE;
        }
    }while(aux);

    if(!strcmp(op,"1")){
        if(nuevoComentario(archCom)){
            VerTema(usuario, nombreTema, fecTema, horaTema);
        }else{
            printf("Comentario no creado");
        }
    }

    if(!strcmp(op,"2")){
        system("clear");
        VerTemas();
    }
}

void CrearTema(){
    int cantTemas=0;
    char *tema=NULL,*ruta=NULL,*fechaTema=NULL,canttema,creado=_FALSE;
    FILE *archivo,*archCant,*archAux;

    tema = (char *) malloc(sizeof(char)* _MAXTEMA);
    ruta = (char *) malloc(sizeof(char)* 15);
    fechaTema = (char *) malloc(sizeof(char)* 128);

    archAux = fopen(ARCHIVOCANTTEMA,"r");
    if(archAux != NULL){
        cantTemas = fgetc(archAux);

        fflush(archAux);
        fclose(archAux);
    }

    strcat(ruta,FOLDERTEMAS);
    strcat(ruta,ARCHIVOTEMA);

    do{
        printf("Ingrese nombre del tema (Máx. 100 caracteres)\n");
        scanf("%*c%[^\n]",tema);
    }while(strlen(tema) > _MAXTEMA);

    archivo = fopen(ruta,"a");
    if(archivo != NULL){
        fputs(d.nombre,archivo);
        fputc('\t',archivo);
        ObtenerFechaFormateada(&fechaTema);
        fputs(fechaTema,archivo);
        fputc('\t',archivo);
        fputc('1',archivo);
        fputc('\n',archivo);
        fputs(tema,archivo);
        fputc('\n',archivo);

        creado = _TRUE;
        cantTemas++;
        canttema=cantTemas;

        archCant = fopen(ARCHIVOCANTTEMA,"w");
        if(archCant != NULL){
            fputc(canttema,archCant);
            
            fflush(archCant);
            fclose(archCant);
        }

        fflush(archivo);
        fclose(archivo);
    }

    free(tema);
    free(ruta);

    system("clear");
    if(creado){
        printf("Tema Creado\n");
    }else{
        printf("Error al crear el tema\n");
    }
    MenuRegistrado(_FALSE);
}

char nuevoComentario(char *nombreArch){
    char *comentario=NULL,*fecha=NULL,*ruta=NULL,ret=_FALSE;
    FILE *archComen;

    comentario = (char *) malloc(sizeof(char)* 1000);
    fecha = (char *) malloc(sizeof(char)* 17);
    ruta = (char *) malloc(sizeof(char)* 115);

    strcpy(ruta,"");
    strcat(ruta,FOLDERTEMAS);
    strcat(ruta,nombreArch);

    printf("Ingrese el comentario del tema (Máx. 1000 caracteres)\n");
    scanf("%*c%[^\n]",comentario);

    archComen = fopen(ruta,"a");
    if(archComen != NULL){
        fputs(d.nombre,archComen);
        fputc('\t',archComen);
        ObtenerFechaFormateada(&fecha);
        fputs(fecha,archComen);
        fputc('\n',archComen);
        fputs(comentario,archComen);
        fputc('\n',archComen);

        fflush(archComen);
        fclose(archComen);

        ret = _TRUE;
    }
    free(comentario);

    return ret;
}

void QuitaEspacios(char **str){
    int i,lon;
    char *vTema=NULL;

    lon = strlen(*str);
    vTema = (char *) malloc(sizeof(char)* lon);
    strcpy(vTema,*str);

    for(i=0;i<lon;i++){
        if(vTema[i] == ' '){
            vTema[i] = '_';
        }
        if(vTema[i] == -56){
            vTema[i] = '_';
        }
        if(vTema[i] == '\"'){
            vTema[i] = '_';
        }
    }

    strcpy(*str,"");
    strcpy(*str,vTema);
}

void ObtenerFechaFormateada(char **fec){
    time_t tiempo = time(0);
    char f[128];

    struct tm *tlocal = localtime(&tiempo);
    strftime(f,128,"%d-%m-%Y %H:%M:%S",tlocal);
    //printf("La hora local es: %s\n",asctime(tlocal));
    //printf( "La hora y fecha locales son: %s\n", fec );

    *fec = f;
}

void ObtenerIP(char **ip){
    /*struct sockaddr whereto;
    struct hostent *hp;
    struct sockaddr_in *to;
    char *target;
    char *hostname;

    memset(&whereto, 0, sizeof(struct sockaddr));
    to = (struct sockaddr_in *)&whereto;
    to->sin_family = AF_INET;
    to->sin_addr.s_addr = inet_addr(target);
    if (to->sin_addr.s_addr != -1)
       hostname = target;
    else{
       hp = gethostbyname(target);
       if(!hp)
          printf("host desconocido%s\n", target);
       else{
          to->sin_family = hp->h_addrtype;
          memcpy(&(to->sin_addr.s_addr), hp->h_addr, hp->h_length);
          hostname = hp->h_name;
        }

          *ip = hostname;
          //printf("gethostbyname ejecutado correctamente\n");
    }*/
}

void Salir(){
    system("EXIT_SUCCESS");
}