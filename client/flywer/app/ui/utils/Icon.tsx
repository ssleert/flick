export interface IconProps {
  "aspect-ratio": [number, number];
  src: string;
  [key: string]: string;
}

// TODO: HYWER EXTENSION
const Icon = ({ "aspect-ratio": [x, y], src, ...attrs }: IconProps) => {
  return <img src={src} style={`aspect-ratio: ${x} / ${y}`} {...attrs} />;
};

export default Icon;
