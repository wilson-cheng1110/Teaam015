import { memo, SVGProps } from 'react';

const Size16Icon = (props: SVGProps<SVGSVGElement>) => (
  <svg preserveAspectRatio='none' viewBox='0 0 14 14' fill='none' xmlns='http://www.w3.org/2000/svg' {...props}>
    <path
      d='M7 0.333333L9.06 4.50667L13.6667 5.18L10.3333 8.42667L11.12 13.0133L7 10.8467L2.88 13.0133L3.66667 8.42667L0.333333 5.18L4.94 4.50667L7 0.333333Z'
      stroke='#1E1E1E'
      strokeWidth={1.6}
      strokeLinecap='round'
      strokeLinejoin='round'
    />
  </svg>
);

const Memo = memo(Size16Icon);
export { Memo as Size16Icon };
