"""Code-drawn homebrew presentation assets, no game/publisher artwork."""
from pathlib import Path
from PIL import Image,ImageDraw,ImageFont
root=Path(__file__).resolve().parent/'sahin/art'; root.mkdir(exist_ok=True)
FONT='/usr/share/fonts/dejavu-sans-fonts/DejaVuSans-Bold.ttf'
if not Path(FONT).exists(): FONT='/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf'
def draw(size,icon=False,logo=False):
    w,h=size; s=w/3840
    image=Image.new('RGBA' if logo else 'RGB',size,(0,0,0,0) if logo else (16,29,22))
    d=ImageDraw.Draw(image)
    if not logo:
        for y in range(h):
            t=y/h; d.line((0,y,w,y),fill=(int(16+19*t),int(29+28*t),int(22+15*t)))
        d.polygon([(0,h*.72),(w*.1,h*.61),(w*.3,h*.64),(w*.55,h*.48),(w*.8,h*.57),(w,h*.41),(w,h),(0,h)],fill=(9,19,13))
        for x,high,bw in ((.04,.28,.07),(.14,.19,.05),(.64,.21,.1),(.78,.35,.07),(.9,.25,.06)):
            d.rectangle((w*x,h*(1-high),w*(x+bw),h),fill=(5,13,8))
        d.rectangle((w*.08,h*.16,w*.1,h*.6),fill=(178,220,131))
    if icon:
        lines=[('GTA',112),('SAN',68),('ANDREAS',48)]
        y=95
        for text,sz in lines:
            f=ImageFont.truetype(FONT,sz); d.text((w/2,y),text,font=f,fill=(244,239,217),anchor='mt',stroke_width=1); y+=sz+12
        d.text((w/2,440),'PS2 CONTROLS · R4',font=ImageFont.truetype(FONT,20),fill=(178,220,131),anchor='mt')
    else:
        x=w*.15; y=h*.24
        for text,sz in [('GRAND THEFT AUTO',108),('SAN ANDREAS',178)]:
            d.text((x,y),text,font=ImageFont.truetype(FONT,int(sz*s)),fill=(244,239,217),stroke_width=max(1,int(s*2))); y+=sz*s*1.32
        if not logo:
            d.text((x,y+35*s),'CLASSIC PC  /  PS5 HOMEBREW  /  PS2 CONTROLS',font=ImageFont.truetype(FONT,int(37*s)),fill=(178,220,131))
    return image
draw((512,512),True).save(root/'gtasa.png')
draw((3840,2160)).save(root/'gtasa-bg.png')
draw((3840,2160),logo=True).save(root/'gtasa-logo.png')
print(root)
