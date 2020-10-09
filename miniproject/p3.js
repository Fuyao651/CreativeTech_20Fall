// By Roni Kaufman

let kMax;
let step;
let radius = 20; // diameter of the circle
let inter = 1; // difference between the sizes of two blobs
let maxNoise = 250;

let noiseProg = (x) => (x);
let weather;
function preload(){
   let url ='http://api.openweathermap.org/data/2.5/weather?q=Los%20angeles&APPID=c4598188bba28f4cb7d055f234a7e790';
   weather = loadJSON(url);
   
}

function setup() {
  createCanvas(windowWidth, windowHeight);
  //setTimeout(preload,1000);
  angleMode(DEGREES);
  noFill();
  //noLoop();
  kMax = random(0.6, 1.0);
  step = 0.01;
  noStroke();
}
 

function draw() {
   let n = weather.main.temp / 10 ; // number of blobs
   
    blendMode(BLEND);
  background(0);
  blendMode(ADD);
  let t = frameCount/150;
  for (let i = n; i > 0; i--) {
    let alpha = pow(1 - noiseProg(i / n), 3);
    let size = radius + i * inter;
    let k = kMax * sqrt(i/n);
    let noisiness = maxNoise * noiseProg(i / n);
    
    fill(255, 0, 0, alpha*255);
    blob(size, width/2, height/2, k, t - i * step, noisiness);
    
    fill(0, 255, 0, alpha*255);
    blob(size, width/2, height/2, k, t - i * step + 0.2, noisiness);
    
    fill(0, 0, 255, alpha*255);
    blob(size, width/2, height/2, k, t - i * step + 0.4, noisiness);
    
   text("Forecast: " + n, 50, 90);
  }
}

function blob(size, xCenter, yCenter, k, t, noisiness) {
  beginShape();
  let angleStep = 360 / 8;
  for (let theta = 0; theta <= 360 + 2 * angleStep; theta += angleStep) {
    let r1, r2;
    /*
    if (theta < PI / 2) {
      r1 = cos(theta);
      r2 = 1;
    } else if (theta < PI) {
      r1 = 0;
      r2 = sin(theta);
    } else if (theta < 3 * PI / 2) {
      r1 = sin(theta);
      r2 = 0;
    } else {
      r1 = 1;
      r2 = cos(theta);
    }
    */
    r1 = cos(theta)+1;
    r2 = sin(theta)+1;
    let r = size + noise(k * r1,  k * r2, t) * noisiness;
    let x = xCenter + r * cos(theta);
    let y = yCenter + r * sin(theta);
    curveVertex(x, y);
  }
  endShape();
}