static const char INDEX[] = "<html> <head><link href=https://maxcdn.bootstrapcdn.com/font-awesome/4.7.0/css/font-awesome.min.css rel=stylesheet integrity=sha384-wvfXpqpZZVQGK6TAh5PVlGOfQNHSoD2xbE+QkPxCAFlNEevoEH3Sl0sibVcOQVnN crossorigin=anonymous><title>CaprerAVR</title><style>\
      html, body {\
        background: #ccc;\
        background: \
        radial-gradient(circle at 100% 50%, transparent 20%, rgba(255,255,255,.3) 21%, rgba(255,255,255,.3) 34%, transparent 35%, transparent),\
        radial-gradient(circle at 0% 50%, transparent 20%, rgba(255,255,255,.3) 21%, rgba(255,255,255,.3) 34%, transparent 35%, transparent) 0 -50px;\
        background-color: #ccc;\
        background-size:75px 100px;\
        font-family: Arial, Helvetica, Sans-Serif;\
        color: #333;\
        height: 100vh;\
        margin: 0;\
        padding: 0;\
      }\
      h1 {\
        color: #c00;\
      }\
      a {\
        color: #333;\
        text-decoration: none;\
      }\
      a:hover {\
        font-weight: bold;\
      }\
      #speaker {\
        position: absolute;\
        top: 50vh;\
        left: 50vw;\
\
        width: 25vw;\
        transform: translate(-50%, -50%);\
      }\
      .btn {\
        display: block;\
        width: 4vw;\
        height: 4vw;\
        margin: 2vw;\
\
        border-radius: 50%;\
        background: black;\
        background: linear-gradient(to bottom right, #444, #666, #000);\
\
        border: 0.5vw solid #333;\
      }\
      .btn * {\
        display: block;\
        width: 100%;\
        height: 100%;\
\
        color: #ccc;\
\
        font-size: 2vw;\
        text-align: center;\
        line-height: 4vw;\
      }\
      .red {\
        background: red;\
        background: linear-gradient(to bottom right, red, #ff6600, red);\
      }\
      .green {\
        background: green;\
        background: linear-gradient(to bottom right, green, #00aa00, green);\
      }\
      .yellow {\
        background: yellow;\
        background: linear-gradient(to bottom right, yellow, #ff7, yellow);\
      }\
      .white {\
        background: white;\
        background: linear-gradient(to bottom right, white, #ccc, white);\
      }\
      #main {\
        display: block;\
        position: absolute;\
        top: 0;\
        margin: 3vw 13vw;\
        padding: 0;\
        width: calc(100% - 26vw);\
        height: calc(100% - 6vw);\
      }\
      #main .btn * {\
        color: #333;\
\
        font-size: 7.9vw;\
        line-height: 10vw;\
      }\
      #main .btn {\
        width: 11vw;\
        height: 11vw;\
      }\
      #main>* {\
        position: absolute;\
      }\
      #main>*:nth-child(1) {\
        top: 0; left: 0;\
      }\
      #main>*:nth-child(2) {\
        top: 0; right: 0;\
      }\
      #main>*:nth-child(3) {\
        bottom: 0; left: 0;\
      }\
      #main>*:nth-child(4) {\
        bottom: 0; right: 0;\
      }\
      #volume, #modes {\
        display: block;\
        position: absolute;\
        top: 50vh;\
        transform: translate(0, -50%);\
        margin: 0;\
        padding: 0;\
      }\
      #volume {\
        right: 0;\
      }\
      #modes {\
        left: 0;\
      }\
    </style></head> <body> <img id=speaker src=https://github.com/ciotto/CaprerAVR/raw/master/images/speaker.png> <ul id=volume> <li class=btn><a href=?volume=up><i class=\"fa fa-volume-up\" aria-hidden=true></i></a></li> <li class=btn><a href=?volume=down><i class=\"fa fa-volume-down\" aria-hidden=true></i></a></li> </ul> <ul id=main> <li class=\"btn green\"><a href=?button=1><i class=\"fa fa-trophy\" aria-hidden=true></i></a></li> <li class=\"btn red\"><a href=?button=2><i class=\"fa fa-times\" aria-hidden=true></i></a></li> <li class=\"btn yellow\"><a href=?button=3><i class=\"fa fa-puzzle-piece\" aria-hidden=true></i></a></li> <li class=\"btn white\"><a href=?button=4><i class=\"fa fa-exclamation-triangle\" aria-hidden=true></i></a></li> </ul> <ul id=modes> <li class=btn><a href=?mode=1>1</a></li> <li class=btn><a href=?mode=2>2</a></li> <li class=btn><a href=?mode=3>3</a></li> <li class=btn><a href=?mode=4>4</a></li> </ul> </body> </html>";