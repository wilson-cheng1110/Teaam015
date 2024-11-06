import { memo, SVGProps } from 'react';

const Arrow_backIcon = (props: SVGProps<SVGSVGElement>) => (
  <svg preserveAspectRatio='none' viewBox='0 0 36 36' fill='none' xmlns='http://www.w3.org/2000/svg' {...props}>
    <path
      d='M8.95417 20.25L21.0875 32.85L18 36L0.666667 18L18 0L21.0875 3.15L8.95417 15.75H35.3333V20.25H8.95417Z'
      fill='#1D1B20'
    />
  </svg>
);

const Memo = memo(Arrow_backIcon);
export { Memo as Arrow_backIcon };
