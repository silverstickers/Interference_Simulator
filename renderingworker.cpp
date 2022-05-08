#include "renderingworker.h"
#include "renderingmaster.h"

#include <cmath>

static float RGB_r [] = {
    0.0014619955811715805,0.0022669056023600243,0.003299036110031063,0.004546896852350439,0.006006105056993791,0.007675918804434457,0.009561203394731096,0.011662968995142865,0.013994707785115502,0.01656105637139426,0.019372742788596516,0.02244719411002579,0.02579282311826555,0.029432387489373633,0.03338491092042817,0.03766747608151851,0.042253067017337005,0.04691458399133113,0.05164425209311529,0.05644871419547314,0.06134044494312783,0.06633085137688166,0.07142826458096291,0.07663702254956387,0.08196142528164385,0.087411214696131,0.09298959953890884,0.0987024276165075,0.1045507907963224,0.11053667232221573,0.11665600122383982,0.12290731198945251,0.12928523040832837,0.13577751583663375,0.14237847430795506,0.1490727384829781,0.1558507266503708,0.1626889760379725,0.16957421944883505,0.17649322158660016,0.18342846019226636,0.19036746731312684,0.19729670678394517,0.20420970925948218,0.21109495319112997,0.21794820252834113,0.22476319518924603,0.2315374529310898,0.2382734320696066,0.2449666980359304,0.2516206637792285,0.2582339375612672,0.2648092188471272,0.27134717156688476,0.27784945989435444,0.2843214014200332,0.2907626953161,0.2971786279866803,0.3035679263767977,0.3099342310464275,0.3162821544923382,0.3226094622720638,0.32892138033063256,0.33521669037969953,0.34150060454122194,0.34777091610802874,0.35403123226470046,0.3602841408353374,0.366528457743285,0.3727683648145053,0.37900068183696356,0.3852285882170877,0.391452904966983,0.3976732251941389,0.4038941281371945,0.4101124478324236,0.41633135152767126,0.4225486702204431,0.42876857492222104,0.43498689244861954,0.4412077984093083,0.447428115165217,0.453650434334324,0.4598753385411611,0.4660996564728312,0.47232856222023284,0.47855787863379246,0.4847897859200844,0.49102210135850827,0.4972564191293403,0.5034933248743276,0.5097296414881407,0.5159675484473075,0.522205863946251,0.5284447718500154,0.5346830866012935,0.5409194040751939,0.5471573097689081,0.5533916265944651,0.5596245324931008,0.565853849035322,0.5720817545692755,0.5783040710826255,0.5845203906718561,0.5907342930011564,0.5969396129909775,0.6031395137899174,0.6093298346523712,0.6155137330265519,0.6216850552354126,0.6278463807173074,0.6339982745553374,0.6401346020827624,0.6462604918955885,0.6523688217818822,0.6584637067856558,0.6645400395062868,0.6705983762227489,0.6766382547106133,0.6826555952415246,0.6886534667732338,0.6946268114822389,0.7005766750861048,0.7065000246724039,0.7123953793054851,0.7182642339188836,0.7241025940115757,0.7299094384556731,0.7356828051012506,0.7414236378808644,0.7471270116731434,0.7527948315414559,0.7584222127903781,0.7640096003184447,0.769556407505972,0.7750588038588068,0.780517595641067,0.7859290015238034,0.7912937764432374,0.7966071922843984,0.8018706152376446,0.8070823747046473,0.8122388089759692,0.8173415491080798,0.8223859952913317,0.8273727148541918,0.8322991733910813,0.8371646398804131,0.8419693412456319,0.846708821213788,0.8513844997023888,0.855991993845198,0.8605336484626919,0.865006157141607,0.8694086743123614,0.873741308408031,0.8780008409852356,0.8821884493306369,0.8863019976673894,0.8903415794934312,0.894305144045262,0.8981917181039129,0.902003277546344,0.9057348680145032,0.9093903996647411,0.9129660070101344,0.9164625104530708,0.9198791350013626,0.9232147698912994,0.9264702498113229,0.9296439014941755,0.9327373523126056,0.9357470217364718,0.9386754432346461,0.9415211303729047,0.9442848284617543,0.9469652258626157,0.9495619408937822,0.9520753085089858,0.9545060414475516,0.9568523791775677,0.9591141303022052,0.9612934380573407,0.9633872061002898,0.9653969856405892,0.9673222685246364,0.9691630662078553,0.970919318954511,0.9725901348141913,0.9741763574084353,0.975677190645675,0.9770920356520277,0.9784222326450961,0.979666095876416,0.9808242624402922,0.9818951440117506,0.982881280054488,0.9837791795198422,0.9845910909683125,0.9853152007413414,0.9859521306674056,0.9865022096405148,0.9869641579064014,0.9873372060008064,0.987622172670732,0.9878191520033638,0.9879261727372323,0.9879451706317489,0.9878741601998459,0.9877141765851464,0.9874641349558363,0.9871241697382241,0.9866942187047217,0.9861751485274827,0.9855662155788282,0.9848651143109276,0.9840751996931422,0.9831960676449997,0.9822281707577935,0.9811732892122574,0.9800321283245658,0.9788062634501479,0.9774970739304246,0.976108224906952,0.9746380082272822,0.9730881744707035,0.9714683557085738,0.9697831130812914,0.968041305354793,0.9662430438023951,0.9643942451078572,0.9625169727369577,0.9606261754860036,0.9587199044763673,0.9568341051627561,0.9549973084887696,0.9532150458875698,0.9515462272506953,0.950018012245954,0.9486831530694507,0.9475940275601823,0.9468091087983127,0.9463921563722028,0.9464031311450462,0.9469030741916794,0.9479372327167105,0.9495450508917115,0.9517404201103928,0.9545291096426641,0.9578957403425837,0.9618122666687167,0.966248777481722,0.9711624870815522,0.9765108962599102,0.9822577545753769,0.9883620799212208
};
static float RGB_g [] = {
    0.0004659913919114934,0.001269897101615975,0.0022490183451722313,0.0033918841632656804,0.004692061241092744,0.006135891503856028,0.0077131160510677,0.0094169153553472,0.011224701548363442,0.01313595377716254,0.01513271743178829,0.017198996785303594,0.01933074318511054,0.02150303695278668,0.023701776954208033,0.025920506649278047,0.028138807463796305,0.030323540520811973,0.03247382729611796,0.03456857075460381,0.03658982977294056,0.038503594004715896,0.04029339894981024,0.04190461115902603,0.043327451450508585,0.04455560411873583,0.04558246689176038,0.0464015612109181,0.04700744862572622,0.04739845362551404,0.04757339371302694,0.04753536748517837,0.04729230240096718,0.046855310502846824,0.04624117675574093,0.04546721023082908,0.044558026492337664,0.0435530737190721,0.042488139902889556,0.04140091975980204,0.04032795978956832,0.03930776663057157,0.03839881405885271,0.03763066869776516,0.0370286796745015,0.036613674395490264,0.03640357374688182,0.03640351817286241,0.03661949264330557,0.03705338654185691,0.037703436097523746,0.038569281262784215,0.039645098963587894,0.040920248896872104,0.04235106621674233,0.043931272334442044,0.04564206683863344,0.04746832221255668,0.04939409900213965,0.05140486616668636,0.0534881537223529,0.05563191003781664,0.057825220219243655,0.060057968826412046,0.06232329248965957,0.06461403589398387,0.06692277585963113,0.06924510666507289,0.07157684449494817,0.0739131776252345,0.07625091481761018,0.07858924726823341,0.08092498458487726,0.08325472094735673,0.08557805215845263,0.08789378949993748,0.09020111672608602,0.09249885580078815,0.09478817843783055,0.09706691940470738,0.09933623724389631,0.10159497990744795,0.10384572179396404,0.10608703735300581,0.10831978115925261,0.11054509253877559,0.11276183809637635,0.11497214601360681,0.11717689277568918,0.11937663835804257,0.12157294554931822,0.12376669200308499,0.12595799741934913,0.1281477445657914,0.1303390492990618,0.1325317964071948,0.13472654160063324,0.1369268480746088,0.13913159264204145,0.14134390081316123,0.14356464417843176,0.14579495531212058,0.1480366969821801,0.15029143569692027,0.15256075186424045,0.1548454880249755,0.15714880993418692,0.15947154274866268,0.1618148719991605,0.1641816008624372,0.1665723257656469,0.16898966363736537,0.17143538357408133,0.17391173185514677,0.17641844614159138,0.17895980652738866,0.1815365142239986,0.18415021667573447,0.18680458958736995,0.1894982847225483,0.1922366729397742,0.19501835963153946,0.19784876490425912,0.20072544237887552,0.20365311312098916,0.20663353472537765,0.20966719575096354,0.21275663751205837,0.21590328739196812,0.2191097508283441,0.2223753888240187,0.22570387532426378,0.22909450162198797,0.23255111985362076,0.236074626914262,0.23966423164432504,0.24332476411029125,0.2470533545534328,0.25085391354890735,0.25472548987486643,0.2586710580118296,0.26268963921769606,0.26678319189183297,0.27095180082645115,0.27519433772522317,0.2795149750247453,0.2839104959497832,0.2883820094161365,0.29293066888944563,0.29755616578042293,0.3022578538064719,0.3070353341236893,0.31189005054876767,0.316819514079576,0.3218239712798184,0.32690370860531126,0.33205714927467816,0.33728491405873967,0.3425833384271254,0.3479551300484793,0.3533965381592675,0.35890794053186803,0.36448975074745,0.37013713800749826,0.3758539732830311,0.38163334525219944,0.3874792047452722,0.39338656173222386,0.3993559137889013,0.40538678902429115,0.41147612778815296,0.4176250250322833,0.42382835018615,0.4300892683482416,0.4364025802485547,0.4427688880476121,0.44918881902600133,0.4556571156219838,0.4621760654003351,0.468741350075834,0.4753543175690524,0.4820115908490303,0.4887145750891598,0.4954598384326913,0.5022460991811288,0.5090760925737746,0.5159433430954236,0.5228513513717987,0.529795592113789,0.5367786143884726,0.5437958464199314,0.5508470769374313,0.5579351064433469,0.5650543281731687,0.5722073701062071,0.5793895836320366,0.5866046371076197,0.5938468430754209,0.6011190480992957,0.608420107478453,0.6157473052058917,0.6231033747802108,0.6304825655882533,0.6378886447799842,0.6453178289458518,0.6527700129448509,0.6602480965501968,0.6677452744424257,0.675265366380739,0.6828045385569567,0.690364638021527,0.6979418049868507,0.7055369718171162,0.7131510743985874,0.7207792367356154,0.7284253454782301,0.7360845034258754,0.7437566173190143,0.7514397717174452,0.7591319265002779,0.7668350414250283,0.7745421938654863,0.7822563112740953,0.7899714619636461,0.7976905802513132,0.8054067300567858,0.8131188812721618,0.8208229971469754,0.8285121501431812,0.836189261303023,0.8438454171701422,0.8514745194008927,0.8590666816140542,0.86662276892152,0.8741269393072385,0.8815661177531517,0.8889401848892272,0.8962233797142661,0.9034074125145412,0.9104706308186431,0.9173976138382822,0.9241658620670481,0.9307581300310862,0.9371570608055302,0.9433453721925603,0.9493162253885609,0.9550605846153878,0.9605853570748163,0.9658937621848641,0.971000188760133,0.9759219036314865,0.980675368253291,0.985280023457573,0.9897505184610552,0.9941071290312428,0.9983616470620554
};
static float RGB_b [] = {
    0.0004659913919114934,0.001269897101615975,0.0022490183451722313,0.0033918841632656804,0.004692061241092744,0.006135891503856028,0.0077131160510677,0.0094169153553472,0.011224701548363442,0.01313595377716254,0.01513271743178829,0.017198996785303594,0.01933074318511054,0.02150303695278668,0.023701776954208033,0.025920506649278047,0.028138807463796305,0.030323540520811973,0.03247382729611796,0.03456857075460381,0.03658982977294056,0.038503594004715896,0.04029339894981024,0.04190461115902603,0.043327451450508585,0.04455560411873583,0.04558246689176038,0.0464015612109181,0.04700744862572622,0.04739845362551404,0.04757339371302694,0.04753536748517837,0.04729230240096718,0.046855310502846824,0.04624117675574093,0.04546721023082908,0.044558026492337664,0.0435530737190721,0.042488139902889556,0.04140091975980204,0.04032795978956832,0.03930776663057157,0.03839881405885271,0.03763066869776516,0.0370286796745015,0.036613674395490264,0.03640357374688182,0.03640351817286241,0.03661949264330557,0.03705338654185691,0.037703436097523746,0.038569281262784215,0.039645098963587894,0.040920248896872104,0.04235106621674233,0.043931272334442044,0.04564206683863344,0.04746832221255668,0.04939409900213965,0.05140486616668636,0.0534881537223529,0.05563191003781664,0.057825220219243655,0.060057968826412046,0.06232329248965957,0.06461403589398387,0.06692277585963113,0.06924510666507289,0.07157684449494817,0.0739131776252345,0.07625091481761018,0.07858924726823341,0.08092498458487726,0.08325472094735673,0.08557805215845263,0.08789378949993748,0.09020111672608602,0.09249885580078815,0.09478817843783055,0.09706691940470738,0.09933623724389631,0.10159497990744795,0.10384572179396404,0.10608703735300581,0.10831978115925261,0.11054509253877559,0.11276183809637635,0.11497214601360681,0.11717689277568918,0.11937663835804257,0.12157294554931822,0.12376669200308499,0.12595799741934913,0.1281477445657914,0.1303390492990618,0.1325317964071948,0.13472654160063324,0.1369268480746088,0.13913159264204145,0.14134390081316123,0.14356464417843176,0.14579495531212058,0.1480366969821801,0.15029143569692027,0.15256075186424045,0.1548454880249755,0.15714880993418692,0.15947154274866268,0.1618148719991605,0.1641816008624372,0.1665723257656469,0.16898966363736537,0.17143538357408133,0.17391173185514677,0.17641844614159138,0.17895980652738866,0.1815365142239986,0.18415021667573447,0.18680458958736995,0.1894982847225483,0.1922366729397742,0.19501835963153946,0.19784876490425912,0.20072544237887552,0.20365311312098916,0.20663353472537765,0.20966719575096354,0.21275663751205837,0.21590328739196812,0.2191097508283441,0.2223753888240187,0.22570387532426378,0.22909450162198797,0.23255111985362076,0.236074626914262,0.23966423164432504,0.24332476411029125,0.2470533545534328,0.25085391354890735,0.25472548987486643,0.2586710580118296,0.26268963921769606,0.26678319189183297,0.27095180082645115,0.27519433772522317,0.2795149750247453,0.2839104959497832,0.2883820094161365,0.29293066888944563,0.29755616578042293,0.3022578538064719,0.3070353341236893,0.31189005054876767,0.316819514079576,0.3218239712798184,0.32690370860531126,0.33205714927467816,0.33728491405873967,0.3425833384271254,0.3479551300484793,0.3533965381592675,0.35890794053186803,0.36448975074745,0.37013713800749826,0.3758539732830311,0.38163334525219944,0.3874792047452722,0.39338656173222386,0.3993559137889013,0.40538678902429115,0.41147612778815296,0.4176250250322833,0.42382835018615,0.4300892683482416,0.4364025802485547,0.4427688880476121,0.44918881902600133,0.4556571156219838,0.4621760654003351,0.468741350075834,0.4753543175690524,0.4820115908490303,0.4887145750891598,0.4954598384326913,0.5022460991811288,0.5090760925737746,0.5159433430954236,0.5228513513717987,0.529795592113789,0.5367786143884726,0.5437958464199314,0.5508470769374313,0.5579351064433469,0.5650543281731687,0.5722073701062071,0.5793895836320366,0.5866046371076197,0.5938468430754209,0.6011190480992957,0.608420107478453,0.6157473052058917,0.6231033747802108,0.6304825655882533,0.6378886447799842,0.6453178289458518,0.6527700129448509,0.6602480965501968,0.6677452744424257,0.675265366380739,0.6828045385569567,0.690364638021527,0.6979418049868507,0.7055369718171162,0.7131510743985874,0.7207792367356154,0.7284253454782301,0.7360845034258754,0.7437566173190143,0.7514397717174452,0.7591319265002779,0.7668350414250283,0.7745421938654863,0.7822563112740953,0.7899714619636461,0.7976905802513132,0.8054067300567858,0.8131188812721618,0.8208229971469754,0.8285121501431812,0.836189261303023,0.8438454171701422,0.8514745194008927,0.8590666816140542,0.86662276892152,0.8741269393072385,0.8815661177531517,0.8889401848892272,0.8962233797142661,0.9034074125145412,0.9104706308186431,0.9173976138382822,0.9241658620670481,0.9307581300310862,0.9371570608055302,0.9433453721925603,0.9493162253885609,0.9550605846153878,0.9605853570748163,0.9658937621848641,0.971000188760133,0.9759219036314865,0.980675368253291,0.985280023457573,0.9897505184610552,0.9941071290312428,0.9983616470620554
};

RenderingWorker::RenderingWorker(RenderingMaster *master, complex corner, complex step,
                                   int width, int height, int yPos,
                                   double angle, int count, double spacing, volatile bool *stopped)
    : m_master(master),
      m_corner(corner),
      m_step(step),
      m_width(width),
      m_height(height),
      m_yPos(yPos),
      m_count(count),
      m_angle(angle),
      m_spacing(spacing),
      stopped(stopped)
{
}

void RenderingWorker::run()
{
    QImage image(m_width, m_height, QImage::Format_RGB32);

    double realStep = std::real(m_step);
    double imagStep = std::imag(m_step);

    double sensor_y = -0.75;

    double x_pos;
    double y_pos = std::imag(m_corner) - m_yPos * imagStep;

    // omega / k = c
    double pi = 3.1416;
    double two_pi = 2*pi;
    double omega = two_pi*40000;
    double k = omega / 340.0;
    double lambda = 340.0 / 40000;
    k = two_pi/lambda;

    double phaseDiff = std::sin(m_angle*pi/180.)*k*m_spacing;
    //phaseDiff = m_angle;
    std::cerr << "phaseDiff: " << phaseDiff << std::endl;
    std::cerr << "m_angle: " << m_angle << " sin(a): " << std::sin(m_angle*pi/180.) << std::endl;

    for (int y_loop = 0; y_loop < m_height; y_loop++)
    {
        if (*stopped) return;

        y_pos -= imagStep;
        x_pos = std::real(m_corner);

        for (int x_loop = 0; x_loop < m_width; x_loop++)
        {
            x_pos += realStep;
            constexpr size_t num_steps = 7;
            double amplitude_array [num_steps] = {0};
            bool nearSensor = false;
            for (int i = 0; i < m_count; i++)
            {
                // exp ( i (w t + k x) = sin(wt + kx) + i cos (wt + kx)
                double sensor_x = m_spacing*( i - (m_count-1)/2.0 );
                auto dx = abs(x_pos - sensor_x);
                auto dy = abs(y_pos - sensor_y);
                auto dx2 = dx*dx;
                auto dy2 = dy*dy;
                auto distance = sqrt(dx2 + dy2);

                // model attenuation as cos(theta) = cos(arctan(d_x/d_y))
                auto attenuation = (y_pos > sensor_y) / sqrt(dx2/dy2 + 1);

                for (size_t step = 0; step < num_steps; step++) {
                    amplitude_array[step] += attenuation * std::sin(k * distance + i*phaseDiff + step*3.1416/num_steps);
                }

                if (distance < 0.002) nearSensor = true;
            }
            // try to find maximum of interference oscillation with #num_steps samples
            auto amplitude = amplitude_array[0]*amplitude_array[0];
            for (size_t cursor = 0; cursor < num_steps; ++cursor) {
                if (amplitude_array[cursor]*amplitude_array[cursor] > amplitude) {
                    amplitude = amplitude_array[cursor]*amplitude_array[cursor];
                }
            }


            qint16 value = static_cast<qint16>(128.0 + amplitude * 127.5/(m_count*m_count));
            QColor color(0, 0, 0);
            if (value < 0 || value > 255) {
                std::cerr << "value: " << value << std::endl;
                std::cerr << "ampl: " << amplitude << std::endl;
            }
            if (!nearSensor) color = QColor::fromRgb(255*RGB_r[255-value], 255*RGB_g[255-value], 255*RGB_b[255-value]);

            image.setPixelColor(x_loop, y_loop, color);
        }
    }
    QMetaObject::invokeMethod(m_master, "handleWorkerResult", Qt::QueuedConnection, Q_ARG(QImage, image), Q_ARG(int, m_yPos));
}
